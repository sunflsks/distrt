#pragma once

#include <functional>
#include <unordered_map>

#include "utils.hpp"

class Material;

class MaterialSerializer {
   public:
    using MaterialTypeId = std::uint8_t;
    using MaterialId = std::uint64_t;
    using DeserializeFunction = std::function<std::shared_ptr<Material>(std::span<std::byte>)>;

    MaterialSerializer() = default;
    MaterialSerializer(std::span<std::byte> bytes_to_deserialize);
    MaterialId id_for_material_instance(std::shared_ptr<Material>& material);
    std::shared_ptr<Material> material_for_id_instance(MaterialId id);
    std::vector<std::byte> bytes();

   private:
    MaterialTypeId id_for_material_type(Material& material);

    // returns a function pointer to the deserialize() method for a given ID
    static DeserializeFunction deserializer_for_type_id(MaterialTypeId id);

    // used for serialization - material -> id
    std::unordered_map<std::shared_ptr<Material>, MaterialId> registered_materials_idx;

    // used for both serialization and deserialization
    std::vector<std::shared_ptr<Material>> registered_materials;
};
