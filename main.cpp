#include <fstream>
#include <iostream>
#include <memory>

#include "camera.hpp"
#include "hittable.hpp"
#include "ray.hpp"
#include "sphere.hpp"

int main() {
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.width = 200;
    cam.viewport_width = 2.0;
    cam.antialiasing_sample_count = 200000;
    cam.output = "output.ppm";

    hittables world;
    world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(std::make_shared<sphere>(point3(0, -100.5, -95), 100));

    cam.render(world);
}
