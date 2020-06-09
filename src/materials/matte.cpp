#include "matte.h"

namespace hikari
{
    void MatteMaterial::ComputeScatteringFunctions(Intersection* si, bool allowMultipleLobes) const {
        si->bsdf = new BSDF(*si);
        Vector3 r = kd;
        LambertianReflection lambertianReflection(r);
        si->bsdf->Add(&lambertianReflection);

    }
} // namespace hikari
