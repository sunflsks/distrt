cmake_minimum_required(VERSION 3.15)
project(distrt VERSION 0.1.0 LANGUAGES CXX)
find_package(OpenMP REQUIRED)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Source files
set(SOURCES
    main.cpp

    camera/camera.cpp

    hittable/hittable.cpp
    hittable/objects/sphere/sphere.cpp

    material/materials/DiffuseLambertian/DiffuseLambertian.cpp
    material/materials/Metal/Metal.cpp

    serialization/register.cpp
    serialization/materials.cpp

    utils/utils.cpp
    utils/vec3.cpp
    utils/socket.cpp

    world/world.cpp
)


add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
target_include_directories(${PROJECT_NAME} PUBLIC .)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror -ggdb3)
