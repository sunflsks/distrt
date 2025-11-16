#include "register.hpp"

// static
int Register::id_ctr = 0;

// static
std::unordered_map<std::type_index, Register::Id> Register::type_to_id;
std::unordered_map<Register::Id, Register::FactoryFunc> Register::id_to_obj;