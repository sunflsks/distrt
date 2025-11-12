
#include "hittable.hpp"

#include "material.hpp"
#include "sphere.hpp"

Hittable::Hittable() = default;
Hittable::Hittable(std::shared_ptr<Material> mat) : mat(std::move(mat)) {}
Hittable::~Hittable() = default;

Hittable::hit_record::hit_record() = default;
Hittable::hit_record::~hit_record() = default;

// need this here so that when we deserialize we can get the ID without instantiating
std::unordered_map<std::type_index, std::byte> Hittable::type_to_id = {
    {typeid(Sphere), std::byte{0x1}}};
