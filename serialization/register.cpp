#include "register.hpp"

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