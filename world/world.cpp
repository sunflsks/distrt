#include "world.hpp"

#include "hittable.hpp"
#include "interval.hpp"
#include "serializer.hpp"

bool World::hit(const Ray& r, const Interval& t_interval, Hittable::hit_record& rec) const {
    bool ok = false;
    double t_closest = t_interval.upper();
    Hittable::hit_record tmp_rec;

    for (auto& tgt : list) {
        if (tgt->hit(r, Interval(t_interval.lower(), t_closest), tmp_rec)) {
            rec = tmp_rec;
            t_closest = rec.t;
            ok = true;
        }
    }

    return ok;
}

std::vector<std::byte> World::bytes() const {
    return WorldSerializer(*this).bytes();
}
