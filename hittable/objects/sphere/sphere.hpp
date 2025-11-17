#pragma once

#include "material/material.hpp"

class Sphere : public Hittable {
   public:
    Sphere(Point3 cen, double r) : Hittable(), center(cen), radius(r) {}

    Sphere(Point3 cen, double r, std::shared_ptr<Material> mat)
        : Hittable(std::move(mat)), center(cen), radius(r) {}

    bool hit(const Ray& r, const Interval& t_interval, hit_record& rec) const override;

    std::vector<std::byte> bytes() const override;
    static std::unique_ptr<Sphere> deserialize(std::span<std::byte> chunk);

   private:
    Point3 center;
    double radius;
};