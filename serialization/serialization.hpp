#include <memory>
#include <vector>

#include "hittable/hittable.hpp"
#include "material/material.hpp"

struct SerializedWorld {
    std::vector<std::shared_ptr<Material>> materials;
    std::vector<std::unique_ptr<Hittable>> objects;
};