#pragma once

#include "materials.hpp"

class metal : public material {
   private:
    color albedo;

   public:
    metal(vec3 albedo) : albedo(albedo){};
    ~metal() override = default;

    std::optional<scatter_record> scatter(const ray& r, const hittable::hit_record& rec) override {
        auto b = r.direction() * rec.normal;  // projection of v onto the unit normal vector
        ray scattered_ray(rec.p, r.direction() * (2*b));
        return scatter_record(scattered_ray, albedo);
    }
};