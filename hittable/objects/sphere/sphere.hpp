#pragma once

#include "material.hpp"

class Sphere : public Hittable {
   public:
    Sphere(Point3 cen, double r, std::shared_ptr<Material> mat)
        : Hittable(std::move(mat)), center(cen), radius(r) {}

    bool hit(const Ray& r, const Interval& t_interval, hit_record& rec) const override;

   private:
    Point3 center;
    double radius;
};