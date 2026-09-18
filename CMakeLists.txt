cmake_minimum_required(VERSION 3.15)
project(distrt VERSION 0.1.0 LANGUAGES CXX)
find_package(OpenMP REQUIRED)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Shared source files across all executables
set(COMMON_SOURCES
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

# Executable targets
add_executable(${PROJECT_NAME} main.cpp ${COMMON_SOURCES})
add_executable(flying_sphere flying_sphere.cpp ${COMMON_SOURCES})

# Target-specific build settings
foreach(TARGET ${PROJECT_NAME} flying_sphere)
    target_link_libraries(${TARGET} PRIVATE OpenMP::OpenMP_CXX)
    target_include_directories(${TARGET} PUBLIC .)
    target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Wpedantic -Werror -freflection)
endforeach()
