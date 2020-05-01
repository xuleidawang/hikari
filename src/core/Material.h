#pragma once

#include "Vectors.h"
#include "Reflection.h"
#include "Intersection.h"

namespace hikari {
    // enum MaterialType {DIFF, SPEC, REFR, GLOSSY, TRANSMISSION};  // material types, used in radiance()
    enum class TransportMode { Radiance, Importance};

class Material{

    public:
    virtual ~Material();
    virtual void ComputeScatteringFunctions(Intersection *intersection, bool allowMultipleLobes) const = 0;

    };

}
