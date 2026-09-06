#pragma once

#include <cstddef>
#include <memory>
#include <span>
#include <type_traits>
#include <vector>

template <typename T>
concept DeserializableUnique = requires() {
    { T::deserialize(std::span<std::byte>()) } -> std::same_as<std::unique_ptr<T>>;
};

template <typename T>
concept DeserializableShared = requires() {
    { T::deserialize(std::span<std::byte>()) } -> std::same_as<std::shared_ptr<T>>;
};

template <typename T>
    requires(std::is_trivially_copyable_v<T>)
void append_to_bytes(std::vector<std::byte>& bytes, const T& object) {
    bytes.insert(bytes.end(),
                 reinterpret_cast<const std::byte*>(&object),
                 reinterpret_cast<const std::byte*>(&object) + sizeof(object));
}

template <typename... Args>
    requires((std::is_trivially_copyable_v<Args> && !std::is_const_v<Args>) && ...)
void deserialize_from_bytes(std::span<std::byte> buf, Args&... args) {
    decltype(buf.size()) offset = 0;

    const auto total_size = (sizeof(Args) + ...);
    if (buf.size_bytes() < total_size) {
        throw std::runtime_error("deserialize_from_bytes: too small");
    }

    (
        [&](auto&& to_ref) {
            std::memcpy(std::addressof(to_ref), buf.data() + offset, sizeof(to_ref));
            offset += sizeof(to_ref);
        }(args),
        ...);
};

inline void add_size_to_bytes(std::vector<std::byte>& bytes, std::uint64_t size) {
    append_to_bytes(bytes, size);
}
