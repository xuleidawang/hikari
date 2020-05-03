#pragma once

#include "Vectors.h"

namespace hikari
{
    Vector2 ConcentricSampleDisk(const Vector2 &u);
    Vector3 UniformSampleHemisphere(const Vector2 &u);
    float UniformHemispherePdf();
    Vector3 UniformSampleSphere(const Vector2 &u);


    inline Vector3 CosineSampleHemisphere(const Vector2 &u)
    {
        Vector2 d = ConcentricSampleDisk(u);
        float z = std::sqrt(std::max((double)0, 1.f - d.x*d.x - d.y*d.y));
        return Vector3(d.x, d.y, z);
    }

} // namespace hikari

