#include "materials.hpp"

#include <stdexcept>

#include "material/materials.hpp"
#include "serialization/utils.hpp"

// to deserialize, we have 2 ids to take care of id 1: material type id: this is the ID of the
// material instance itself. may be shared between objects, likely not. these ids will be generated
// as the order they are received. id 2: material type id - this is the ID of the type itself. as of
// now, only 2 - more incoming.

/*
class MaterialSerializer {
   public:
    using MaterialTypeId = std::uint8_t;
    using MaterialId = std::uint64_t;

    std::uint64_t id_for_material(std::shared_ptr<Material>& material);
    std::shared_ptr<Material> material_for_id(MaterialId id);

   private:
    using DeserializeFunction = std::function<std::shared_ptr<Material>(std::span<std::byte>)>;

    std::uint64_t id_for_material_type(Material& material);

    // returns a function pointer to the deserialize() method for a given ID
    DeserializeFunction deserializer_for_type_id(MaterialTypeId id);

    // used for serialization
    std::unordered_map<std::shared_ptr<Material>, std::uint64_t> registered_materials;
};
*/

// These are internal and are only used for actually deducing the material type
namespace {
std::unordered_map<std::type_index, MaterialSerializer::MaterialTypeId> type_to_id = {
    {std::type_index(typeid(DiffuseLambertian)), 1}, {std::type_index(typeid(Metal)), 2}};

std::unordered_map<MaterialSerializer::MaterialTypeId, std::type_index> id_to_type = {
    {1, std::type_index(typeid(DiffuseLambertian))}, {2, std::type_index(typeid(Metal))}};
}  // namespace

MaterialSerializer::MaterialId MaterialSerializer::id_for_material_instance(
    std::shared_ptr<Material>& material) {
    if (!registered_materials_idx.contains(material)) {
        registered_materials.push_back(material);
        registered_materials_idx[material] = registered_materials.size() - 1;
    }

    return registered_materials_idx[material];
}

MaterialSerializer::MaterialSerializer(std::span<std::byte> bytes_to_deserialize) {
	/*
	 	* while not empty, go through bytes_to_deserialize. 
		* first std::uint64_t is mat id
		* second std::uint64_t is size
		* push deserialize_from_bytes into registered_materials
	 */

	while (!bytes_to_deserialize.empty()) {
		MaterialTypeId id = *reinterpret_cast<MaterialTypeId*>(bytes_to_deserialize.data());
		bytes_to_deserialize = bytes_to_deserialize.subspan(sizeof(id));

		std::uint64_t size = *reinterpret_cast<std::uint64_t*>(bytes_to_deserialize.data());
		bytes_to_deserialize = bytes_to_deserialize.subspan(sizeof(size));

		auto new_mat = MaterialSerializer::deserializer_for_type_id(id)(bytes_to_deserialize.subspan(0, size));
		bytes_to_deserialize = bytes_to_deserialize.subspan(size);

		registered_materials.push_back(std::move(new_mat));
	}
}

std::shared_ptr<Material> MaterialSerializer::material_for_id_instance(MaterialId id) {
    if (registered_materials.size() <= id) {
        throw std::runtime_error(
            std::format("Invalid id {} passed to material_instance_for_id while deserializing",
                        id));
    }

    return registered_materials[id];
}

std::vector<std::byte> MaterialSerializer::bytes() {
    std::vector<std::byte> all_materials;

    for (auto& material : registered_materials) {
        auto material_id = id_for_material_type(*material);
        auto material_bytes = material->bytes();

        append_to_bytes(all_materials, material_id);
        add_size_to_bytes(all_materials, material_bytes.size());
        all_materials.insert(all_materials.end(),
                             std::make_move_iterator(material_bytes.begin()),
                             std::make_move_iterator(material_bytes.end()));
    }

    return all_materials;
}

// private
MaterialSerializer::MaterialTypeId MaterialSerializer::id_for_material_type(Material& material) {
    return type_to_id[std::type_index(typeid(material))];
}

// private
MaterialSerializer::DeserializeFunction MaterialSerializer::deserializer_for_type_id(
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
