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
    int c = 0;
    for (double i = 0; i < 0.7; i += 0.01, c++) {  // movie loop
        camera cam;
        cam.output = std::format("scene_{}.ppm", c);

        hittables world;
        auto material_ground = std::make_shared<diffuse_lambertian>(color(0.2, 0.8, 0.0));
        auto material_center = std::make_shared<diffuse_lambertian>(color(0.1, 0.2, 0.5));
        auto material_left = std::make_shared<metal>(color(0.8, 0.8, 0.8));
        auto material_right = std::make_shared<metal>(color(0.8, 0.6, 0.2));

        world.add(std::make_unique<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
        world.add(std::make_unique<sphere>(point3(0.0, 0.0 + (i * 5), -1.2), 0.5, material_center));
        world.add(std::make_unique<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
        world.add(std::make_unique<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

        cam.render(world);
    }
}
