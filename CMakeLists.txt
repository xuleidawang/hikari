cmake_minimum_required(VERSION 3.12)
project(PathTracer)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(./src)
include_directories(./src/core)
include_directories(lib/tiny_obj_loader)

set(CMAKE_CXX_COMPILER /usr/local/bin/g++-9)
set(CMAKE_C_COMPILER /usr/local/bin/gcc-9)
# enable openmp using -fopenmp
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fopenmp")

add_executable(PathTracer
        lib/tiny_obj_loader/tiny_obj_loader.cc
        lib/tiny_obj_loader/tiny_obj_loader.h
        src/Bounds.h
        src/core/Camera.h
        src/core/Intersection.h
        src/main.cpp
        src/core/Material.h
        src/core/Shape.h
        src/core/Ray.h
        src/core/Scene.h
        src/shapes/Sphere.h
        src/shapes/Triangle.h
        src/Vectors.h
        src/shapes/Sphere.cpp
        src/accelerators/BVH.h
        src/accelerators/BVH.cpp
        src/core/Reflection.h
        src/core/Reflection.cpp
        src/core/Integrator.h
        README.md src/integrators/whitted.h src/integrators/whitted.cpp src/core/Primitive.cpp src/core/Primitive.h src/core/Light.cpp src/core/Light.h src/main/hikari.cpp src/core/hikari.h src/core/sampler.cpp src/core/sampler.h src/core/parser.cpp src/core/parser.h src/core/paramset.cpp src/core/paramset.h src/core/Intersection.cpp src/core/Integrator.cpp src/core/fileutil.cpp src/core/fileutil.h src/core/api.h src/core/api.cpp)
