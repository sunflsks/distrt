cmake_minimum_required(VERSION 3.15)
project(distrt VERSION 0.1.0 LANGUAGES CXX)
find_package(OpenMP REQUIRED)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    main.cpp
    camera/camera.cpp
    hittable/hittable.cpp
    hittable/hittables.cpp
    hittable/objects/sphere/sphere.cpp
    utils/utils.cpp
    utils/vec3.cpp
)


add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
target_include_directories(${PROJECT_NAME} PUBLIC camera hittable hittable/objects/sphere material utils .)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -ggdb3)