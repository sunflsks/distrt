#pragma once

#include "material.hpp"

class sphere : public hittable {
   public:
    sphere(point3 cen, double r, std::shared_ptr<material> mat)
        : hittable(std::move(mat)), center(cen), radius(r) {}

    bool hit(const ray& r, const interval& t_interval, hit_record& rec) const override;

   private:
    point3 center;
    double radius;
};