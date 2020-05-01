#include "matte.h"

namespace hikari
{
    void MatteMaterial::ComputeScatteringFunctions(Intersection* si, bool allowMultipleLobes) const {
        Vector3 r = kd;
        si->bsdf->Add(&LambertianReflection(r));
    }
    
} // namespace hikari
