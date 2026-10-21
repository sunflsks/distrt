#pragma once
#include "material/material.hpp"
#include "serialization/record_types.hpp"
#include "utils/validation.hpp"

class DiffuseLambertian : public Material {
   private:
    Color albedo;

   public:
    struct Record {
        serialization::Triple albedo;

        void validate() const { validation::require_finite(albedo); }
    };

    explicit DiffuseLambertian(const Record& record)
        : DiffuseLambertian(Color(record.albedo[0], record.albedo[1], record.albedo[2])) {
        record.validate();
    }

    Record to_record() const { return {{albedo.x(), albedo.y(), albedo.z()}}; }

    DiffuseLambertian(Vec3 albedo) : albedo(albedo) {};
    ~DiffuseLambertian() override = default;

    std::optional<ScatterRecord> scatter([[maybe_unused]] const Ray& r,
                                         const Hittable::hit_record& rec) override;
};
