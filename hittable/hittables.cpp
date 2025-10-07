#include "hittable.hpp"

bool hittables::hit(const ray& r, const interval& t_interval, hittable::hit_record& rec) const {
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