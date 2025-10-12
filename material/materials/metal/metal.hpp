#pragma once

#include "materials.hpp"

class metal : public material {
   private:
    color albedo;
    double fuzz_factor;

   public:
    metal(vec3 albedo, double fuzz_factor = 0.25) : albedo(albedo), fuzz_factor(fuzz_factor){};
    ~metal() override = default;

    std::optional<scatter_record> scatter(const ray& r, const hittable::hit_record& rec) override {
        // v - 2(v . n)n
        auto reflection = r.direction() - 2 * (r.direction().dot(rec.normal)) * rec.normal;

        // implement fuzz - get random unit vector
        auto fuzz_addition = ray::rand_unit_vec() * fuzz_factor;
        reflection += fuzz_addition;

        auto new_ray = ray(rec.p, reflection);

        return scatter_record(new_ray, albedo);
    }
};