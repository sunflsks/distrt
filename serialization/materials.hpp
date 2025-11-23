#include <functional>
#include <unordered_map>

#include "material/material.hpp"
#include "utils.hpp"

class MaterialSerializer {
   public:
    using MaterialTypeId = std::uint8_t;
    using MaterialId = std::uint64_t;
    using DeserializeFunction = std::function<std::shared_ptr<Material>(std::span<std::byte>)>;

    MaterialId id_for_material(std::shared_ptr<Material>& material);
    std::shared_ptr<Material> material_for_id(MaterialId id);

   private:
    MaterialTypeId id_for_material_type(Material& material);

    // returns a function pointer to the deserialize() method for a given ID
    DeserializeFunction deserializer_for_type_id(MaterialTypeId id);

    // used for serialization
    std::unordered_map<std::shared_ptr<Material>, std::uint64_t> registered_materials;
};
