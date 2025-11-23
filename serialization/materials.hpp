#include <unordered_map>

#include "material/materials.hpp"

class MaterialSerializer {
   public:
    inline std::uint64_t register_material(std::shared_ptr<Material>& material) {
        auto material_ptr = material.get();

        if (registered_materials.contains(material_ptr)) {
            return registered_materials[material_ptr];
        }

        registered_materials[material_ptr] = ++current_id;
        return current_id;
    }

    std::vector<std::byte> bytes() {
        
    }

    std::shared_ptr<Material> material_for_id(std::uint64_t id) { return registered_materials[id]; }

   private:
    std::uint64_t current_id = 0;
    std::unordered_map<Material*, std::uint64_t> registered_materials;
};