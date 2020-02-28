cmake_minimum_required(VERSION 3.12)
project(PathTracer)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(lib/tiny_obj_loader)

set(CMAKE_CXX_COMPILER /usr/local/bin/g++-9)
set(CMAKE_C_COMPILER /usr/local/bin/gcc-9)
# enable openmp using -fopenmp
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fopenmp")

add_executable(PathTracer
        lib/tiny_obj_loader/tiny_obj_loader.cc
        lib/tiny_obj_loader/tiny_obj_loader.h
        src/Bounds3.h
        src/core/Camera.h
        src/core/Intersection.h
        src/main.cpp
        src/core/Material.h
        src/core/Shape.h
        src/core/Ray.h
        README.md
        src/core/Scene.h
        src/shapes/Sphere.h
        src/shapes/Triangle.h
        src/Vectors.h
        src/shapes/Sphere.cpp
        src/accelerators/BVH.h
        src/accelerators/BVH.cpp src/core/Reflection.h src/core/Reflection.cpp src/core/Integrator.h)
