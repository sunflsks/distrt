#include "material/material.hpp"

class DiffuseLambertian : public Material {
   private:
    Color albedo;

   public:
    DiffuseLambertian(Vec3 albedo) : albedo(albedo) {};
    ~DiffuseLambertian() override = default;

    std::optional<ScatterRecord> scatter([[maybe_unused]] const Ray& r,
                                         const Hittable::hit_record& rec) override;
    std::vector<std::byte> bytes() const override;

    static std::shared_ptr<Material> deserialize(std::span<std::byte> bytes);
};
