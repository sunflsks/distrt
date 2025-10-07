// all this bogus needed for unique_ptr with incomplete type

#include "hittable.hpp"

#include "material.hpp"

hittable::hittable() = default;
hittable::hittable(std::shared_ptr<material> mat) : mat(std::move(mat)) {}
hittable::~hittable() = default;

hittable::hit_record::hit_record() = default;
hittable::hit_record::~hit_record() = default;
