#pragma once

#include <array>
#include <bit>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <meta>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <variant>
#include <vector>

#include "utils/validation.hpp"

namespace serialization::binary {

class Writer {
    std::vector<std::byte>& output;

   public:
    explicit Writer(std::vector<std::byte>& output) : output(output) {}
    void byte(std::uint8_t value) { output.push_back(static_cast<std::byte>(value)); }
};

class Reader {
    const std::vector<std::byte>& input;
    std::size_t position = 0;

   public:
    explicit Reader(const std::vector<std::byte>& input) : input(input) {}
    std::uint8_t byte() {
        if (position == input.size()) throw std::runtime_error("Truncated scene");
        return std::to_integer<std::uint8_t>(input[position++]);
    }
};

// type traits
template <class T>
struct is_array : std::false_type {};
template <class T, std::size_t N>
struct is_array<std::array<T, N>> : std::true_type {};
template <class T>

struct is_vector : std::false_type {};
template <class T>
struct is_vector<std::vector<T>> : std::true_type {};
template <class T>

struct is_optional : std::false_type {};
template <class T>
struct is_optional<std::optional<T>> : std::true_type {};
template <class T>

struct is_variant : std::false_type {};
template <class... T>
struct is_variant<std::variant<T...>> : std::true_type {};

// specializations declare the record types and their stable on-disk tags
template <class T>
struct RecordTraits;

template <class T>
concept Record = requires { RecordTraits<T>::tag; };

template <class T>
void write(Writer& out, const T& value);

template <class T>
T read(Reader& in);

template <class Variant, std::size_t I = 0>
Variant read_variant(Reader& in, std::uint8_t tag) {
    if constexpr (I == std::variant_size_v<Variant>) {
        throw std::runtime_error("Unknown scene type tag");
    }

    else {
        using Alternative = std::variant_alternative_t<I, Variant>;
        if (tag == RecordTraits<Alternative>::tag)
            return Variant(std::in_place_type<Alternative>, read<Alternative>(in));

        return read_variant<Variant, I + 1>(in, tag);
    }
}

template <class T>
void write(Writer& out, const T& value) {
    if constexpr (std::unsigned_integral<T> && !std::same_as<T, bool>) {
        for (std::size_t i = 0; i < sizeof(T); ++i)
            out.byte(static_cast<std::uint8_t>(value >> (i * 8)));
    }

    else if constexpr (std::same_as<T, double>) {
        static_assert(sizeof(double) == 8 && std::numeric_limits<double>::is_iec559);
        if (!validation::finite(value)) throw std::runtime_error("Non-finite scene value");
        write(out, std::bit_cast<std::uint64_t>(value));
    }

    else if constexpr (is_optional<T>::value) {
        write(out, std::uint8_t(value.has_value()));
        if (value) write(out, *value);
    }

    else if constexpr (is_array<T>::value || is_vector<T>::value) {
        if constexpr (is_vector<T>::value) {
            write(out, static_cast<std::uint32_t>(value.size()));
        }
        for (const auto& element : value) write(out, element);
    }

    else if constexpr (is_variant<T>::value) {
        std::visit(
            [&](const auto& record) {
                using R = std::remove_cvref_t<decltype(record)>;
                write(out, std::uint8_t(RecordTraits<R>::tag));
                write(out, record);
            },
            value);
    }

    else if constexpr (Record<T>) {
        template for (constexpr auto member :
                      std::define_static_array(std::meta::nonstatic_data_members_of(
                          ^^T,
                          std::meta::access_context::current()))) {
            write(out, value.[:member:]);
        }
    }

    else {
        static_assert(sizeof(T) == 0, "Unsupported binary scene type");
    }
}

template <class T>
T read(Reader& in) {
    if constexpr (std::unsigned_integral<T> && !std::same_as<T, bool>) {
        T value = 0;
        for (std::size_t i = 0; i < sizeof(T); ++i) value |= static_cast<T>(in.byte()) << (i * 8);
        return value;
    }

    else if constexpr (std::same_as<T, double>) {
        const double value = std::bit_cast<double>(read<std::uint64_t>(in));
        if (!validation::finite(value)) throw std::runtime_error("Non-finite scene value");
        return value;
    }

    else if constexpr (is_optional<T>::value) {
        const auto present = read<std::uint8_t>(in);
        if (present > 1) throw std::runtime_error("Invalid optional presence flag");
        if (!present) return std::nullopt;
        return read<typename T::value_type>(in);
    }

    else if constexpr (is_array<T>::value) {
        T value{};
        for (auto& element : value) element = read<typename T::value_type>(in);
        return value;
    }

    else if constexpr (is_vector<T>::value) {
        const auto count = read<std::uint32_t>(in);
        T value;
        for (std::uint32_t i = 0; i < count; ++i) value.push_back(read<typename T::value_type>(in));
        return value;
    }

    else if constexpr (is_variant<T>::value) {
        return read_variant<T>(in, read<std::uint8_t>(in));
    }

    else if constexpr (Record<T>) {
        T value{};
        template for (constexpr auto member :
                      std::define_static_array(std::meta::nonstatic_data_members_of(
                          ^^T,
                          std::meta::access_context::current()))) {
            using Field = std::remove_cvref_t<decltype(value.[:member:])>;
            value.[:member:] = read<Field>(in);
        }
        return value;
    }

    else {
        static_assert(sizeof(T) == 0, "Unsupported binary scene type");
    }
}
}  // namespace serialization::binary
