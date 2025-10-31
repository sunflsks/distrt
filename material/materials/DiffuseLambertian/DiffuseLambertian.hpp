#include "material.hpp"

class DiffuseLambertian : public Material {
   private:
    Color albedo;

   public:
    DiffuseLambertian(Vec3 albedo) : albedo(albedo){};
    ~DiffuseLambertian() override = default;

    std::optional<ScatterRecord> scatter([[maybe_unused]] const Ray& r,
                                         const Hittable::hit_record& rec) override {
        auto rand_vec = Ray::rand_unit_vec() + rec.normal;
        return ScatterRecord{Ray(rec.p, rand_vec), albedo};
    }
};