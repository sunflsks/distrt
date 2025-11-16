#pragma once
#include <cstddef>
#include <functional>
#include <typeinfo>

#include "hittable.hpp"

class Register {
   public:
    using Id = std::byte;

    static std::unique_ptr<Hittable> make_hittable(Id id, std::span<std::byte> data) {
        return id_to_obj[id](data);
    }

    static Id hittable_id(const Hittable& hittable) { return type_to_id[typeid(hittable)]; }

    template <typename T>
    static bool register_hittable() {
        static_assert(std::is_convertible_v<T*, Hittable*>);

        Id id = static_cast<Id>(++id_ctr);

        type_to_id[typeid(T)] = id;
        id_to_obj[id] = &T::deserialize;

        return true;
    }

   private:
    using FactoryFunc = std::function<std::unique_ptr<Hittable>(std::span<std::byte>)>;

    static int id_ctr;

    static std::unordered_map<std::type_index, Id> type_to_id;
    static std::unordered_map<Id, FactoryFunc> id_to_obj;
};
