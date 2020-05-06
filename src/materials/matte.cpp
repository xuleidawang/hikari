#include "matte.h"

namespace hikari
{
    void MatteMaterial::ComputeScatteringFunctions(Intersection* si, bool allowMultipleLobes) const {
        Vector3 r = kd;
        LambertianReflection lambertianReflection(r);
        si->bsdf->Add(&lambertianReflection);

    }
} // namespace hikari
