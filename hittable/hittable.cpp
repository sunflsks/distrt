
#include "hittable.hpp"

#include "material.hpp"
#include "materials/DiffuseLambertian/DiffuseLambertian.hpp"
#include "sphere.hpp"

Hittable::Hittable() : mat(std::make_shared<DiffuseLambertian>(Vec3(0, 0, 0))){};
Hittable::Hittable(std::shared_ptr<Material> mat) : mat(mat) {}
Hittable::~Hittable() = default;

Hittable::hit_record::hit_record() = default;
Hittable::hit_record::~hit_record() = default;