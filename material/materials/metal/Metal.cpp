#include "Metal.hpp"

#include "serialization/utils.hpp"

std::optional<ScatterRecord> Metal::scatter(const Ray& r, const Hittable::hit_record& rec) {
    // v - 2(v . n)n
    auto reflection = r.direction() - 2 * (r.direction().dot(rec.normal)) * rec.normal;

    // implement fuzz - get random unit vector
    auto fuzz_addition = Ray::rand_unit_vec() * fuzz_factor;
    reflection += fuzz_addition;

    auto new_ray = Ray(rec.p, reflection);

    return ScatterRecord(new_ray, albedo);
}

std::vector<std::byte> Metal::bytes() const {
    std::vector<std::byte> bytes;
    add_size_to_bytes(bytes, static_cast<uint64_t>(sizeof(albedo)));
    append_to_bytes(bytes, albedo);
    return bytes;
}
