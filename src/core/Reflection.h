//
// Created by LEI XU on 12/3/18.
//

#ifndef PATHTRACER_REFLECTION_H
#define PATHTRACER_REFLECTION_H

#include "hikari.h"
#include "Bounds.h"
#include "Ray.h"
#include "Shape.h"

namespace  hikari {
    // BSDF Declarations
    enum BxDFType {
        BSDF_REFLECTION = 1 << 0,
        BSDF_TRANSMISSION = 1 << 1,
        BSDF_DIFFUSE = 1 << 2,
        BSDF_GLOSSY = 1 << 3,
        BSDF_SPECULAR = 1 << 4,
        BSDF_ALL = BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR | BSDF_REFLECTION |
                   BSDF_TRANSMISSION,
    };

    class BxDF {
    public:
        virtual ~BxDF(){}
        BxDF(MaterialType type) : type(type) {}
        //bool MatchesFlags(MaterialType t) const { return (type & t) == type; }
        virtual Vector3 f(const Vector3 &wo, const Vector3 &wi) const = 0;
        virtual Vector3 Sample_f(const Vector3 &wo, Vector3 *wi, const Vector2 &sample, float *pdf, BxDFType sampledType) const;
        virtual Vector3 rho(const Vector3 &wo, int nSamples, const Vector2 *samples) const;
        virtual Vector3 rho(int nSamples, const Vector2 *samples1, const Vector2 *samples2) const;
        virtual double Pdf(const Vector3 &wo, const Vector3 &wi) const;
        virtual std::string ToString() const = 0;

        // BxDF Public Data
        const MaterialType type;
    };


    class BRDF: public BxDF{

    };


    class BTDF: public  BxDF{

    };


}








#endif //PATHTRACER_REFLECTION_H
