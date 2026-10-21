#pragma once

#include <cstddef>
#include <vector>

#include "world/world.hpp"

namespace serialization {
// Returns the binary scene payload, suitable for file or network transport.
std::vector<std::byte> serialize(const World& world);
// Input must contain a complete scene. Malformed or truncated data throws.
World deserialize(const std::vector<std::byte>& input);
}  // namespace serialization
