#pragma once

#include <array>
#include <bit>
#include <cstdint>
#include <limits>
#include <stdexcept>

namespace validation {
// Representation checks remain reliable under -Ofast's finite-math assumptions.
inline bool finite(double value) {
    static_assert(sizeof(double) == 8 && std::numeric_limits<double>::is_iec559);
    return (std::bit_cast<std::uint64_t>(value) & 0x7ff0000000000000ULL) != 0x7ff0000000000000ULL;
}

template <std::size_t N>
void require_finite(const std::array<double, N>& values) {
    for (double value : values)
        if (!finite(value)) throw std::runtime_error("Non-finite scene value");
}
}  // namespace validation
