#include "DiffuseLambertian.hpp"

std::optional<ScatterRecord> DiffuseLambertian::scatter([[maybe_unused]] const Ray& r,
                                                        const Hittable::hit_record& rec) {
    auto rand_vec = Ray::rand_unit_vec() + rec.normal;
    return ScatterRecord{Ray(rec.p, rand_vec), albedo};
}
