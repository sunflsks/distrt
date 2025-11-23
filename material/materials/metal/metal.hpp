#pragma once

#include "material/materials.hpp"

class Metal : public Material {
   private:
    Color albedo;
    double fuzz_factor;

   public:
    Metal(Vec3 albedo, double fuzz_factor = 0.25) : albedo(albedo), fuzz_factor(fuzz_factor) {};
    ~Metal() override = default;
    std::optional<ScatterRecord> scatter(const Ray& r, const Hittable::hit_record& rec) override;
    std::vector<std::byte> bytes() const override;
};
