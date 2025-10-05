#pragma once

#include <memory>
#include <vector>

#include "ray.hpp"
#include "utils.hpp"

class hittable {
   public:
    class hit_record {
       public:
        point3 p;     // point of intersection
        vec3 normal;  // normal at the intersection
        double t;     // ray parameter at intersection
        bool inside;  // is the intersection inside the object
    };

    virtual bool hit(const ray& r, const interval& t_interval,
                     hit_record& rec) const = 0;
    virtual ~hittable() = default;
};

class hittables : public hittable {
   private:
    std::vector<std::unique_ptr<hittable>> list;

   public:
    void add(std::unique_ptr<hittable> tgt) { list.push_back(std::move(tgt)); }

    bool hit(const ray& r, const interval& t_interval,
             hittable::hit_record& rec) const override {
        bool ok = false;
        double t_closest = t_interval.upper();
        hittable::hit_record tmp_rec;

        for (auto& tgt : list) {
            if (tgt->hit(r, interval(t_interval.lower(), t_closest), tmp_rec)) {
                rec = tmp_rec;
                t_closest = rec.t;
                ok = true;
            }
        }

        return ok;
    }
};