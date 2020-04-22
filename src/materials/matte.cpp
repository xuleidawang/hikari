#include "Reflection.h"
#include "Intersection.h"

namespace hikari
{
    void MatteMaterial::ComputeScatteringFunctions(Intersection* si, bool allowMultipleLobes) const{
        Vector3 r = kd;
        si->bsdf->Add(LambertianReflection(r));
    }
    
    MatteMaterial *CreateMatteMaterial(const Vector3 kd){returnn new MatteMaterial(kd);}
} // namespace hikari
