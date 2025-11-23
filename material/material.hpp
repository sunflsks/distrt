#pragma once

#include "hittable/hittable.hpp"
#include "utils/ray.hpp"

struct ScatterRecord {
    Ray scattered;
    Color attenuation;
};

class Material {
   public:
    virtual std::optional<ScatterRecord> scatter(const Ray& r, const Hittable::hit_record& rec) = 0;
    virtual std::vector<std::byte> bytes() const = 0;
    virtual ~Material() = default;
};
