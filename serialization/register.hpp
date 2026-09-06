#pragma once
#include <cstddef>

#include "hittable/hittable.hpp"

class Register {
   public:
    using Id = std::uint8_t;

    static std::unique_ptr<Hittable> make_hittable(Id id, std::span<std::byte> data, MaterialSerializer& mat_serialier);
    static Id hittable_id(const Hittable& hittable);
};
