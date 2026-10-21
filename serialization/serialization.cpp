#include "serialization.hpp"

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>
#include <variant>
#include <vector>

#include "binary.hpp"
#include "hittable/objects/sphere/sphere.hpp"
#include "material/materials.hpp"

namespace serialization::binary {
template <>
struct RecordTraits<DiffuseLambertian::Record> {
    static constexpr std::uint8_t tag = 1;
};

template <>
struct RecordTraits<Metal::Record> {
    static constexpr std::uint8_t tag = 2;
};

template <>
struct RecordTraits<Sphere::Record> {
    static constexpr std::uint8_t tag = 1;
};
}  // namespace serialization::binary

namespace serialization {
namespace {
using MaterialRecord = std::variant<DiffuseLambertian::Record, Metal::Record>;
using ObjectRecord = std::variant<Sphere::Record>;

struct SerializedWorld {
    std::uint32_t version = 1;
    std::vector<MaterialRecord> materials;
    std::vector<ObjectRecord> objects;
};

void validate(const SerializedWorld& record) {
    if (record.version != 1) {
        throw std::runtime_error("Unsupported scene version");
    }

    for (const auto& material : record.materials) {
        std::visit([](const auto& data) { data.validate(); }, material);
    }

    for (const auto& object : record.objects) {
        std::visit(
            [&](const auto& data) {
                data.validate();

                if (data.material_id && *data.material_id >= record.materials.size()) {
                    throw std::runtime_error("Invalid material ID");
                }
            },
            object);
    }
}

MaterialRecord material_record(const Material& material) {
    if (typeid(material) == typeid(DiffuseLambertian)) {
        return static_cast<const DiffuseLambertian&>(material).to_record();
    }

    if (typeid(material) == typeid(Metal)) {
        return static_cast<const Metal&>(material).to_record();
    }

    throw std::runtime_error("Unsupported material type");
}

SerializedWorld to_record(const World& world) {
    SerializedWorld record;
    std::unordered_map<const Material*, MaterialId> material_ids;
    for (const auto& object : world.objects()) {
        if (!object) throw std::runtime_error("Null world object");

        // just for now - as we add more objects, we will need to refactor this
        const auto* sphere = dynamic_cast<const Sphere*>(object.get());
        if (!sphere || typeid(*object) != typeid(Sphere)) {
            throw std::runtime_error("Unsupported object type");
        }

        std::optional<MaterialId> id;
        if (object->mat) {
            auto it = material_ids.find(object->mat.get());
            if (it == material_ids.end()) {
                const auto next = static_cast<MaterialId>(record.materials.size());
                record.materials.push_back(material_record(*object->mat));
                it = material_ids.emplace(object->mat.get(), next).first;
            }
            id = it->second;
        }

        record.objects.emplace_back(sphere->to_record(id));
    }
    validate(record);
    return record;
}

World from_record(const SerializedWorld& record) {
    validate(record);
    std::vector<std::shared_ptr<Material>> materials;
    for (const auto& material : record.materials) {
        materials.push_back(std::visit(
            [](const auto& data) -> std::shared_ptr<Material> {
                using R = std::remove_cvref_t<decltype(data)>;
                if constexpr (std::same_as<R, DiffuseLambertian::Record>)
                    return std::make_shared<DiffuseLambertian>(data);

                else
                    return std::make_shared<Metal>(data);
            },
            material));
    }

    World world;
    for (const auto& object : record.objects) {
        std::visit(
            [&](const auto& data) {
                auto material = data.material_id ? materials.at(*data.material_id) : nullptr;
                world.add(std::make_unique<Sphere>(data, std::move(material)));
            },
            object);
    }
    return world;
}

void write_record(const SerializedWorld& record, std::vector<std::byte>& output) {
    validate(record);
    binary::Writer out(output);
    // ASCII "DSRT", followed by version, material table, and object table.
    binary::write(out, std::uint32_t{0x54525344});
    binary::write(out, record.version);
    binary::write(out, record.materials);
    binary::write(out, record.objects);
}

SerializedWorld read_record(const std::vector<std::byte>& input) {
    binary::Reader in(input);
    if (binary::read<std::uint32_t>(in) != 0x54525344)
        throw std::runtime_error("Invalid scene magic");
    SerializedWorld record;
    record.version = binary::read<std::uint32_t>(in);
    if (record.version != 1) throw std::runtime_error("Unsupported scene version");
    record.materials = binary::read<std::vector<MaterialRecord>>(in);
    record.objects = binary::read<std::vector<ObjectRecord>>(in);
    validate(record);
    return record;
}

}  // namespace

std::vector<std::byte> serialize(const World& world) {
    std::vector<std::byte> output;
    write_record(to_record(world), output);
    return output;
}

World deserialize(const std::vector<std::byte>& input) {
    return from_record(read_record(input));
}
}  // namespace serialization
