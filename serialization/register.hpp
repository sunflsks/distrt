#pragma once
#include <cstddef>
#include <functional>
#include <typeinfo>

#include "hittable.hpp"

class Register {
   public:
    using Id = std::byte;
    using FactoryFunc = std::function<std::unique_ptr<Hittable>(std::span<std::byte>)>;

    static std::unique_ptr<Hittable> make_hittable(Id id, std::span<std::byte> data);
    static Id hittable_id(const Hittable& hittable);

    // we make these functions so that we know when the static vars get initialized
    static std::unordered_map<std::type_index, Id>& get_type_to_id();
    static std::unordered_map<Id, FactoryFunc>& get_id_to_obj();

    template <typename T>
    static bool register_hittable() {
        static_assert(std::is_convertible_v<T*, Hittable*>);

        Id id = static_cast<Id>(++id_ctr);
        std::cout << static_cast<int>(id) << std::endl;

        get_type_to_id()[typeid(T)] = id;
        get_id_to_obj()[id] = &T::deserialize;

        return true;
    }

   private:
    static int id_ctr;
};
