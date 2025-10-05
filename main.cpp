#include <fstream>
#include <iostream>
#include <memory>

#include "camera.hpp"
#include "hittable.hpp"
#include "ray.hpp"
#include "sphere.hpp"

int main() {
    camera cam;

    hittables world;
    world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

    cam.render(world);
}
