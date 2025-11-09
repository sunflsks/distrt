#pragma once

#include <memory>
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
    virtual ~Hittable();
};

class Hittables : public Hittable {
   private:
    std::vector<std::unique_ptr<Hittable>> list;

   public:
    void add(std::unique_ptr<Hittable> tgt) { list.push_back(std::move(tgt)); }

    bool hit(const Ray& r, const Interval& t_interval, Hittable::hit_record& rec) const override;
};