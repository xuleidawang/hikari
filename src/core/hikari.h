//
// Created by LEI XU on 3/6/20.
//

#ifndef PATHTRACER_HIKARI_H
#define PATHTRACER_HIKARI_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <string>

namespace hikari {
    // Global Forward Declarations
    class Vector3;
    class Vector2;
    class Scene;
    class Integrator;
    class SamplerIntegrator;
    class Ray;
    class Bounds;
    struct Intersection;
    class Shape;
    class Sphere;
    class Primitive;
    class GeometricPrimitive;
    class Camera;
    class Sampler;
    class BxDF;
    class BRDF;
    class BTDF;
    class BSDF;
    class Material;
    class Light;
    class VisibilityTester;
    class AreaLight;
    class ParamSet;

    const double OneMinusEpsilon = 0x1.fffffffffffffp-1;
}

#endif //PATHTRACER_HIKARI_H
