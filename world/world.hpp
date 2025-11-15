#pragma once

#include <vector>

#include "hittable.hpp"

class World : public Hittable {
    friend class WorldSerializer;

   private:
    std::vector<std::unique_ptr<Hittable>> list;

   public:
    void add(std::unique_ptr<Hittable> tgt) { list.push_back(std::move(tgt)); }
    bool hit(const Ray& r, const Interval& t_interval, Hittable::hit_record& rec) const override;
    std::vector<std::byte> bytes() const override;
};