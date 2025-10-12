#include <format>
#include <fstream>
#include <iostream>
#include <memory>

#include "camera.hpp"
#include "hittable.hpp"
#include "materials.hpp"
#include "ray.hpp"
#include "sphere.hpp"

int main() {
    auto material_ground = std::make_shared<diffuse_lambertian>(color(0.2, 0.8, 0.0));
    auto material_center = std::make_shared<diffuse_lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = std::make_shared<metal>(color(0.8, 0.8, 0.8));
    auto material_right = std::make_shared<metal>(color(0.8, 0.6, 0.2));

    for (int i = 0; i < 75; i++) {
        camera cam;
        hittables world;

        cam.output = std::format("scene_{}.ppm", i);

        double modifier = i / ((double)75 / 2);

        world.add(std::make_unique<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
        world.add(
            std::make_unique<sphere>(point3(0.0, 0.0 + (0.1 * 5), -1.2), 0.5, material_center));
        world.add(std::make_unique<sphere>(point3(-1.0, modifier, -1.0), 0.5, material_left));
        world.add(std::make_unique<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

        cam.vfov = 20 + i;
        cam.center = point3(2, 2, 1);
        cam.render(world);
    }
}
