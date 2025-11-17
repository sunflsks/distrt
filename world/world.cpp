#include "world.hpp"

#include "hittable/hittable.hpp"
#include "hittable/objects/sphere/sphere.hpp"
#include "serialization/register.hpp"
#include "utils/interval.hpp"

bool World::hit(const Ray& r, const Interval& t_interval, Hittable::hit_record& rec) const {
    bool ok = false;
    double t_closest = t_interval.upper();
    Hittable::hit_record tmp_rec;

    for (auto& tgt : list) {
        if (tgt->hit(r, Interval(t_interval.lower(), t_closest), tmp_rec)) {
            rec = tmp_rec;
            t_closest = rec.t;
            ok = true;
        }
    }

    return ok;
}

// SERIALIZATION

std::vector<std::byte> World::bytes() const {
    // format
    // std::byte (uint8_t) type
    // std::byte*x (uint32_t) size
    // info.....
    // rinse and repeat till we hit the end

    std::vector<std::byte> serialized_objs;

    for (auto& hittable : list) {
        auto type_id = std::byte{Register::hittable_id(*hittable)};
        serialized_objs.push_back(type_id);

        auto bytes = hittable->bytes();
        serialized_objs.insert(serialized_objs.end(),
                               std::make_move_iterator(bytes.begin()),
                               std::make_move_iterator(bytes.end()));
    }

    return serialized_objs;
}

void World::deserialize(std::vector<std::byte> bytes) {
    // assuming bytes is a vector that only holds the data we want - no more.

    auto cur = bytes.data();
    auto end = bytes.data() + bytes.size();

    while (cur < end) {
        auto id = *reinterpret_cast<std::uint8_t*>(cur);
        cur += sizeof(std::uint8_t);

        uint64_t pack_size = *reinterpret_cast<uint64_t*>(cur);
        cur += sizeof(uint64_t);

        std::span<std::byte> data_span(cur, pack_size);

        list.push_back(Register::make_hittable(id, data_span));

        cur += pack_size;
    }
}