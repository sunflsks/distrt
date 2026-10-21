#pragma once

#include <memory>
#include <vector>

#include "hittable/hittable.hpp"

class World : public Hittable {
   private:
    std::vector<std::unique_ptr<Hittable>> list;

   public:
    const std::vector<std::unique_ptr<Hittable>>& objects() const { return list; }

    void add(std::unique_ptr<Hittable> tgt) { list.push_back(std::move(tgt)); }
    bool hit(const Ray& r, const Interval& t_interval, Hittable::hit_record& rec) const override;
};
