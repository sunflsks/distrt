#include "Metal.hpp"

#include <stdexcept>

std::optional<ScatterRecord> Metal::scatter(const Ray& r, const Hittable::hit_record& rec) {
    // v - 2(v . n)n
    auto reflection = r.direction() - 2 * (r.direction().dot(rec.normal)) * rec.normal;

    // implement fuzz - get random unit vector
    auto fuzz_addition = Ray::rand_unit_vec() * fuzz_factor;
    reflection += fuzz_addition;

    auto new_ray = Ray(rec.p, reflection);

    return ScatterRecord(new_ray, albedo);
}
