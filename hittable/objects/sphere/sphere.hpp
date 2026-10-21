#pragma once

#include <optional>
#include <utils/ray.hpp>

#include "hittable/hittable.hpp"
#include "serialization/record_types.hpp"
#include "utils/validation.hpp"

class Sphere : public Hittable {
   public:
    struct Record {
        serialization::Triple center;
        double radius;
        std::optional<serialization::MaterialId> material_id;

        void validate() const { validation::require_finite(center); }
    };

    Sphere(const Record& record, std::shared_ptr<Material> material)
        : Sphere(Point3(record.center[0], record.center[1], record.center[2]), record.radius,
                 std::move(material)) {
        record.validate();
    }

    Record to_record(std::optional<serialization::MaterialId> material_id) const {
        return {{center.x(), center.y(), center.z()}, radius, material_id};
    }

    Sphere() : Hittable() {}

    Sphere(Point3 cen, double r) : Hittable(), center(cen), radius(r) {}

    Sphere(Point3 cen, double r, std::shared_ptr<Material> mat)
        : Hittable(std::move(mat)), center(cen), radius(r) {}

    bool hit(const Ray& r, const Interval& t_interval, hit_record& rec) const override;

   private:
    Point3 center;
    double radius;
};
