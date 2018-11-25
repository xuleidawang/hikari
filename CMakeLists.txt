cmake_minimum_required(VERSION 3.12)
project(PathTracer)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(lib/tiny_obj_loader)

set(CMAKE_CXX_COMPILER /usr/local/bin/g++-8)
set(CMAKE_C_COMPILER /usr/local/bin/gcc-8)
# enable openmp using -fopenmp
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_executable(PathTracer
        lib/tiny_obj_loader/tiny_obj_loader.cc
        lib/tiny_obj_loader/tiny_obj_loader.h
        Bounds3.h
        Camera.h
        Intersection.h
        main.cpp
        Material.h
        Object.h
        Ray.h
        README.md
        Scene.h
        Sphere.h
        Triangle.h
        Vectors.h)
