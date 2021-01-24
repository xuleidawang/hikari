cmake_minimum_required(VERSION 3.12)
project(Hikari)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(.)
include_directories(./src)
include_directories(./src/core)
include_directories(lib/tiny_obj_loader)

set(CMAKE_CXX_COMPILER /usr/local/bin/g++-9)
set(CMAKE_C_COMPILER /usr/local/bin/gcc-9)
# enable openmp using -fopenmp
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fopenmp")

add_executable(Hikari
        src/main/hikari.cpp
        lib/tiny_obj_loader/tiny_obj_loader.cc
        lib/tiny_obj_loader/tiny_obj_loader.h
        lib/minipbrt.h
        lib/minipbrt.cpp
        src/Bounds.h
        src/core/Camera.h
        src/core/Intersection.h
        src/core/Material.h
        src/core/Shape.h
        src/core/Ray.h
        src/core/Scene.h
        src/core/Primitive.h
        src/core/Light.h
        src/core/Reflection.h
        src/core/Integrator.h

        src/Vectors.h

        src/shapes/Sphere.h
        src/shapes/Sphere.cpp
        src/shapes/Triangle.h
        src/shapes/Triangle.cpp

        src/accelerators/BVH.h
        src/accelerators/BVH.cpp

        src/core/Reflection.cpp
        src/core/Camera.cpp
        src/core/Scene.cpp
        src/core/Primitive.cpp 
        src/core/Light.cpp 
        src/core/hikari.h 
        src/core/sampler.cpp 
        src/core/sampler.h 
        src/core/sampling.h
        src/core/sampling.cpp
        src/core/Intersection.cpp 
        src/core/Integrator.cpp 
        src/core/Material.cpp

        src/integrators/whitted.h 
        src/integrators/whitted.cpp 

        src/materials/matte.h
        src/materials/matte.cpp
        
        README.md)
