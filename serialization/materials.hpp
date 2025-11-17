#include <unordered_set>

#include "material/materials.hpp"

class MaterialSerializer {
   public:
    std::uint32_t register_material(std::shared_ptr<Material>& mat) { registered_materials.insert(mat); }

   private:
    std::unordered_set<std::shared_ptr<Material>> registered_materials;
}