#pragma once

#include <cstddef>
#include <vector>

#include "world.hpp"

class WorldSerializer {
   public:
    WorldSerializer() = delete;
    WorldSerializer(const World& world) : world(world){};

    std::vector<std::byte> bytes() const;

   private:
    const World& world;
};