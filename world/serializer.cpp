#include "serializer.hpp"

#include "world.hpp"

std::vector<std::byte> WorldSerializer::bytes() {
    // format
    // std::byte (uint8_t) type
    // std::byte*x (uint32_t) size
    // info.....
    // rinse and repeat till we hit the end

    std::vector<std::byte> serialized_objs;

    for (auto& hittable : world.list) {
        serialized_objs.insert(serialized_objs.end(),
                               std::make_move_iterator(hittable->bytes().begin()),
                               std::make_move_iterator(hittable->bytes().end()));
    }
}