#include "materials.hpp"

#include <stdexcept>

#include "material/materials.hpp"

// to deserialize, we have 2 ids to take care of
// id 1: material type id: this is the ID of the material instance itself. may be shared between
// objects, likely not. these ids will be generated as the order they are received.
// id 2: material type id - this is the ID of the type itself. as of now, only 2 - more incoming.

/*
class MaterialSerializer {
   public:
    using MaterialTypeId = std::uint8_t;
    using MaterialId = std::uint64_t;

    std::uint64_t id_for_material(std::shared_ptr<Material>& material);
    std::shared_ptr<Material> material_for_id(MaterialId id);

   private:
    using DeserializeFunction = std::function<std::shared_ptr<Hittable>(std::span<std::byte>)>;

    std::uint64_t id_for_material_type(Material& material);

    // returns a function pointer to the deserialize() method for a given ID
    DeserializeFunction deserializer_for_type_id(MaterialTypeId id);

    // used for serialization
    std::unordered_map<std::shared_ptr<Material>, std::uint64_t> registered_materials;
};
*/

namespace {
std::unordered_map<std::type_index, MaterialSerializer::MaterialTypeId> type_to_id = {
    {std::type_index(typeid(DiffuseLambertian)), 1}, {std::type_index(typeid(Metal)), 2}};

std::unordered_map<MaterialSerializer::MaterialTypeId, std::type_index> id_to_type = {
    {1, std::type_index(typeid(DiffuseLambertian))}, {2, std::type_index(typeid(Metal))}};
}  // namespace

std::uint64_t id_for_material(std::shared_ptr<Material>& material);
std::shared_ptr<Material> material_for_id(std::uint64_t id);

// private
MaterialSerializer::MaterialTypeId MaterialSerializer::id_for_material_type(Material& material) {
    return type_to_id[std::type_index(typeid(material))];
}

MaterialSerializer::DeserializeFunction deserializer_for_type_id(
    MaterialSerializer::MaterialTypeId id) {
    auto it = id_to_type.find(id);
    if (it == id_to_type.end()) {
        throw std::runtime_error("deserializer_for_type_id: could not parse the given ID");
    }

    auto type = it->second;

    if (type == typeid(DiffuseLambertian)) {
        return &DiffuseLambertian::deserialize;
    }

    if (type == typeid(Metal)) {
        return &Metal::deserialize;
    }

    throw std::runtime_error("Invalid type id passed to deserializer_for_type_id");
}
