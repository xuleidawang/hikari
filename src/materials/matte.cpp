#include "matte.h"
#include "Reflection.h"
#include "Intersection.h"

namespace hikari
{
    MatteMaterial::MatteMaterial(Vector3 kd) {
        this->kd = kd;
    }
    void MatteMaterial::ComputeScatteringFunctions(Intersection* si, bool allowMultipleLobes) const {
        Vector3 r = kd;
        si->bsdf->Add(LambertianReflection(r));
    }
    
    MatteMaterial *CreateMatteMaterial(const Vector3 kd){return new MatteMaterial(kd);}
} // namespace hikari
