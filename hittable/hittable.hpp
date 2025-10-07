#pragma once

#include <memory>
#include <vector>

#include "ray.hpp"
#include "utils.hpp"

class material;

class hittable {
   public:
    std::shared_ptr<material> mat;

    class hit_record {
       public:
        std::shared_ptr<material> mat;
        point3 p;     // point of intersection
        vec3 normal;  // normal at the intersection
        double t;     // ray parameter at intersection
        bool inside;  // is the intersection inside the object

        hit_record();
        ~hit_record();
    };

    hittable();
    hittable(std::shared_ptr<material> mat);

    virtual bool hit(const ray& r, const interval& t_interval, hit_record& rec) const = 0;
    virtual ~hittable();
};

class hittables : public hittable {
   private:
    std::vector<std::unique_ptr<hittable>> list;

   public:
    void add(std::unique_ptr<hittable> tgt) { list.push_back(std::move(tgt)); }

    bool hit(const ray& r, const interval& t_interval, hittable::hit_record& rec) const override;
};