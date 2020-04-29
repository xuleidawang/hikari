#include "hikari.h"
#include "Material.h"

#ifndef MATTE_MATERIAL_H
#define MATTE_MATERIAL_H

namespace hikari
{
    //MatteMaterial Declarations
    class MatteMaterial : public Material{
        public:

        MatteMaterial(Vector3 kd);
        void ComputeScatteringFunctions(Intersection *si, bool allowMultiplelobes)const;

        private:
        Vector3 kd;

    };
    MatteMaterial *CreateMatteMaterial(const Vector3 kd);
} // namespace hikari

#endif