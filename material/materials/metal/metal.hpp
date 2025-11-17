#pragma once

#include "material/materials.hpp"

class Metal : public Material {
   private:
    Color albedo;
    double fuzz_factor;

   public:
    Metal(Vec3 albedo, double fuzz_factor = 0.25) : albedo(albedo), fuzz_factor(fuzz_factor){};
    ~Metal() override = default;

    std::optional<ScatterRecord> scatter(const Ray& r, const Hittable::hit_record& rec) override {
        // v - 2(v . n)n
        auto reflection = r.direction() - 2 * (r.direction().dot(rec.normal)) * rec.normal;

        // implement fuzz - get random unit vector
        auto fuzz_addition = Ray::rand_unit_vec() * fuzz_factor;
        reflection += fuzz_addition;

        auto new_ray = Ray(rec.p, reflection);

        return ScatterRecord(new_ray, albedo);
    }
};