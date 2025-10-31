// all this bogus needed for unique_ptr with incomplete type

#include "hittable.hpp"

#include "material.hpp"

Hittable::Hittable() = default;
Hittable::Hittable(std::shared_ptr<Material> mat) : mat(std::move(mat)) {}
Hittable::~Hittable() = default;

Hittable::hit_record::hit_record() = default;
Hittable::hit_record::~hit_record() = default;
