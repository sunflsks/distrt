#include "DiffuseLambertian.hpp"

#include "serialization/utils.hpp"

constexpr uint64_t DL_SIZE = sizeof(Color);

std::optional<ScatterRecord> DiffuseLambertian::scatter([[maybe_unused]] const Ray& r,
                                                        const Hittable::hit_record& rec) {
    auto rand_vec = Ray::rand_unit_vec() + rec.normal;
    return ScatterRecord{Ray(rec.p, rand_vec), albedo};
}

std::vector<std::byte> DiffuseLambertian::bytes() const {
    std::vector<std::byte> byte_rep;

    append_to_bytes(byte_rep, DL_SIZE);
    append_to_bytes(byte_rep, albedo);

    return byte_rep;
}
