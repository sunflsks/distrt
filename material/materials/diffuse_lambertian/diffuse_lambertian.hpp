#include "material.hpp"

class diffuse_lambertian : public material {
   private:
    color albedo;

   public:
    diffuse_lambertian(vec3 albedo) : albedo(albedo){};
    ~diffuse_lambertian() override = default;

    std::optional<scatter_record> scatter(const ray& r,
                                          const hittable::hit_record& rec) override {
        auto rand_vec = ray::rand_unit_vec() + rec.normal;
        return scatter_record{ray(rec.p, rand_vec), albedo};
    }
};