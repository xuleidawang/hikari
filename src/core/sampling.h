#pragma once

#include "Vectors.h"

namespace hikari
{
    inline Vector3 CosineSampleHemisphere(const Vector2 &u)
    {
        Vector2 d = ConcentricSampleDisk(u);
        float z = std::sqrt(std::max((float)0, 1- d.x*d.x - d.y*d.y));
        return Vector3(d.x, d.y, z);
    }

} // namespace hikari

