#include "register.hpp"

#include "hittable/objects/sphere/sphere.hpp"

// static
std::unique_ptr<Hittable> Register::make_hittable(Register::Id id, std::span<std::byte> data, MaterialSerializer& mat_serializer) {
    switch (id) {
        case 1:
            return Sphere::deserialize(data, mat_serializer);
    }

    throw std::runtime_error("Given incompatible id to deserialize");
}

// static
Register::Id Register::hittable_id(const Hittable& hittable) {
    if (dynamic_cast<const Sphere*>(&hittable) != nullptr) {
        return 1;
    }

    throw std::runtime_error("Given incompatible hittable to serialize");
}
