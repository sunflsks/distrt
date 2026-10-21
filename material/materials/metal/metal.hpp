#pragma once

#include "material/material.hpp"
#include "serialization/record_types.hpp"
#include "utils/validation.hpp"

class Metal : public Material {
   private:
    Color albedo;
    double fuzz_factor;

   public:
    struct Record {
        serialization::Triple albedo;
        double fuzz_factor;

        void validate() const { validation::require_finite(albedo); }
    };

    explicit Metal(const Record& record)
        : Metal(Color(record.albedo[0], record.albedo[1], record.albedo[2]), record.fuzz_factor) {
        record.validate();
    }

    Record to_record() const { return {{albedo.x(), albedo.y(), albedo.z()}, fuzz_factor}; }

    Metal(Vec3 albedo, double fuzz_factor = 0.25) : albedo(albedo), fuzz_factor(fuzz_factor) {};
    ~Metal() override = default;
    std::optional<ScatterRecord> scatter(const Ray& r, const Hittable::hit_record& rec) override;
};
