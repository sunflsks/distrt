#pragma once

#include <memory>
#include <span>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "ray.hpp"
#include "utils/interval.hpp"

class Material;

class Hittable {
   public:
    std::shared_ptr<Material> mat;

    class hit_record {
       public:
        std::shared_ptr<Material> mat;
        Point3 p;     // point of intersection
        Vec3 normal;  // normal at the intersection
        double t;     // ray parameter at intersection
        bool inside;  // is the intersection inside the object

        hit_record();
        ~hit_record();
    };

    Hittable();
    Hittable(std::shared_ptr<Material> mat);

    virtual bool hit(const Ray& r, const Interval& t_interval, hit_record& rec) const = 0;

    virtual std::vector<std::byte> bytes() const = 0;

    virtual ~Hittable();
};
