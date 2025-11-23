#pragma once
#include <cstddef>
#include <functional>

#include "hittable/hittable.hpp"

template <typename T>
concept RegistrableHittable = std::derived_from<T, Hittable> && requires() {
    { T::deserialize(std::span<std::byte>()) } -> std::same_as<std::unique_ptr<T>>;
};

class Register {
   public:
    using Id = std::uint8_t;
    using FactoryFunc = std::function<std::unique_ptr<Hittable>(std::span<std::byte>)>;

    // we use these two functions so that we know when the static vars get initialized
    inline static std::unique_ptr<Hittable> make_hittable(Register::Id id,
                                                          std::span<std::byte> data) {
        return get_id_to_obj()[id](data);
    }

    inline static Register::Id hittable_id(const Hittable& hittable) {
        return get_type_to_id()[typeid(hittable)];
    }

    template <RegistrableHittable T>
    static bool register_hittable() {
        static_assert(std::is_convertible_v<T*, Hittable*>);

        Id id = static_cast<Id>(++id_ctr);
        std::cout << static_cast<int>(id) << std::endl;

        get_type_to_id()[typeid(T)] = id;
        get_id_to_obj()[id] = &T::deserialize;

        return true;
    }

   private:
    inline static int id_ctr = 0;

    static std::unordered_map<std::type_index, Register::Id>& get_type_to_id();
    static std::unordered_map<Register::Id, Register::FactoryFunc>& get_id_to_obj();
};
