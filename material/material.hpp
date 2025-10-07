#pragma once

#include "hittable.hpp"
#include "ray.hpp"

struct scatter_record {
    ray scattered;
    color attenuation;
};

class material {
   public:
    virtual std::optional<scatter_record> scatter(const ray& r, const hittable::hit_record& rec) {
        return std::nullopt;
    }
    virtual ~material() = default;
};
