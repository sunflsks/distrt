#include "register.hpp"

// static
int Register::id_ctr = 0;

// static
std::unique_ptr<Hittable> Register::make_hittable(Register::Id id, std::span<std::byte> data) {
    return get_id_to_obj()[id](data);
}

// static
Register::Id Register::hittable_id(const Hittable& hittable) {
    return get_type_to_id()[typeid(hittable)];
}

// static
std::unordered_map<std::type_index, Register::Id>& Register::get_type_to_id() {
    static std::unordered_map<std::type_index, Id> type_to_id;
    return type_to_id;
}

// static
std::unordered_map<Register::Id, Register::FactoryFunc>& Register::get_id_to_obj() {
    static std::unordered_map<Id, FactoryFunc> id_to_obj;
    return id_to_obj;
}