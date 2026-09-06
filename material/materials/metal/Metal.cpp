#include "Metal.hpp"

#include <stdexcept>

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
    append_to_bytes(bytes, albedo);
    append_to_bytes(bytes, fuzz_factor);
    return bytes;
}

// static
std::shared_ptr<Material> Metal::deserialize(std::span<std::byte> bytes) {
    if (bytes.size_bytes() != sizeof(albedo) + sizeof(fuzz_factor)) {
        throw std::runtime_error("Invalid span passed to Metal::deserialize");
    }

    double fuzz_factor;
    Vec3 albedo;

    deserialize_from_bytes(bytes, albedo, fuzz_factor);

    return std::make_shared<Metal>(albedo, fuzz_factor);
}
