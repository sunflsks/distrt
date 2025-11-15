#include "serializer.hpp"

#include "world.hpp"

std::vector<std::byte> WorldSerializer::bytes() const {
    // format
    // std::byte (uint8_t) type
    // std::byte*x (uint32_t) size
    // info.....
    // rinse and repeat till we hit the end

    std::vector<std::byte> serialized_objs;

    for (auto& hittable : world.list) {
        auto bytes = hittable->bytes();
        serialized_objs.insert(serialized_objs.end(),
                               std::make_move_iterator(bytes.begin()),
                               std::make_move_iterator(bytes.end()));
    }

    return serialized_objs;
}