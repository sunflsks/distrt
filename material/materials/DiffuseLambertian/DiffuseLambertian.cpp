#include "DiffuseLambertian.hpp"

#include "serialization/utils.hpp"

std::optional<ScatterRecord> DiffuseLambertian::scatter([[maybe_unused]] const Ray& r,
                                                        const Hittable::hit_record& rec) {
    auto rand_vec = Ray::rand_unit_vec() + rec.normal;
    return ScatterRecord{Ray(rec.p, rand_vec), albedo};
}

std::vector<std::byte> DiffuseLambertian::bytes() const {
    std::vector<std::byte> byte_rep;

    append_to_bytes(byte_rep, albedo);

    return byte_rep;
}

// static
std::shared_ptr<Material> DiffuseLambertian::deserialize(std::span<std::byte> bytes) {
    if (bytes.size_bytes() != sizeof(Color)) {
	std::cout << bytes.size_bytes() << std::endl;
        throw std::runtime_error("Invalid span passed to DiffuseLambertian::deserialize");
    }

    Vec3 albedo;
    deserialize_from_bytes(bytes, albedo);
    return std::make_shared<DiffuseLambertian>(albedo);
}
