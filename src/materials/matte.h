
#pragma once

#include "Material.h"

namespace hikari
{
    //MatteMaterial Declarations
    class MatteMaterial : public Material{
    public:
        MatteMaterial(Vector3 kd){this->kd = kd;}
        ~MatteMaterial(){};
        void ComputeScatteringFunctions(Intersection *intersection, bool allowMultipleLobes) const;
        
    private:
        Vector3 kd;
    };
} // namespace hikari
