cmake_minimum_required(VERSION 3.12)
project(PathTracer)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(lib/tiny_obj_loader)

add_executable(PathTracer
        lib/tiny_obj_loader/tiny_obj_loader.cc
        lib/tiny_obj_loader/tiny_obj_loader.h
        obj/dragon2.obj
        results/diff+spe128.ppm
        results/diff+spe256.ppm
        results/diff+spe4.ppm
        results/diffuse128.ppm
        results/diffuse4.ppm
        results/diffuse64spp.ppm
        results/diffuSpeGlossy256.ppm
        results/diffuSpeGlossy64spp.ppm
        results/image.ppm
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
