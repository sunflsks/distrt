#include <format>
#include <fstream>
#include <iostream>
#include <memory>

#include "camera/camera.hpp"
#include "hittable/hittable.hpp"
#include "hittable/objects/sphere/sphere.hpp"
#include "material/materials.hpp"
#include "utils/ray.hpp"
#include "world/world.hpp"

int main() {
    auto material_ground = std::make_shared<DiffuseLambertian>(Color(0.2, 0.8, 0.0));
    auto material_center = std::make_shared<DiffuseLambertian>(Color(0.1, 0.2, 0.5));
    auto material_left = std::make_shared<Metal>(Color(0.8, 0.8, 0.8));
    auto material_right = std::make_shared<Metal>(Color(0.8, 0.6, 0.2));

    Camera cam;
    cam.center = Point3(2, 2, 1);
    cam.output = "scene.ppm";
    for (int i = 0; i < 1500; i++) {
        World world;

        double modifier = i / ((double)75 / 2);

        world.add(std::make_unique<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
        world.add(
            std::make_unique<Sphere>(Point3(0.0, 0.0 + (0.1 * 5), -1.2), 0.5, material_center));
        world.add(std::make_unique<Sphere>(Point3(-1.0, modifier, -1.0), 0.5, material_left));
        world.add(std::make_unique<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

        cam.vfov = 20 + i;
        cam.render(world);
    }
}
