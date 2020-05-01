//
// Created by LEI XU on 12/3/18.
//

#pragma once

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
        BSDF_ALL = BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR | BSDF_REFLECTION |BSDF_TRANSMISSION
        };
        
class BSDF {
    public:
    // BSDF Public Methods
    BSDF(const Intersection &si, float eta = 1) : eta(eta) {}
    void Add(BxDF *b) {
        bxdfs[nBxDFs++] = b;
    }
    int NumComponents(BxDFType flags = BSDF_ALL) const;

    Vector3 f(const Vector3 &woW, const Vector3 &wiW,
               BxDFType flags = BSDF_ALL) const;
    Vector3 rho(int nSamples, const Vector2 *samples1, const Vector2 *samples2,
                 BxDFType flags = BSDF_ALL) const;
    Vector3 rho(const Vector3 &wo, int nSamples, const Vector2 *samples,
                 BxDFType flags = BSDF_ALL) const;
    Vector3 Sample_f(const Vector3 &wo, Vector3 *wi, const Vector2 &u,
                      float *pdf, BxDFType type = BSDF_ALL,
                      BxDFType *sampledType = nullptr) const;
    float Pdf(const Vector3 &wo, const Vector3 &wi,
              BxDFType flags = BSDF_ALL) const;
    std::string ToString() const;

    // BSDF Public Data
    const float eta;

  private:
    // BSDF Private Methods
    ~BSDF() {}

    // BSDF Private Data
    //ns: shading normal, ng: geometry normal
    const Vector3 ns, ng;
    const Vector3 ss, ts;
    int nBxDFs = 0;
    static const int MaxBxDFs = 8;
    BxDF *bxdfs[MaxBxDFs];
    // friend class MixMaterial;
};

class BxDF {
    public:
        virtual ~BxDF(){}
        BxDF(BxDFType type) : type(type) {}
        bool MatchesFlags(BxDFType t) const { return (type & t) == type; }
        virtual Vector3 f(const Vector3 &wo, const Vector3 &wi) const = 0;
        virtual Vector3 Sample_f(const Vector3 &wo, Vector3 *wi, const Vector2 &sample, float *pdf, BxDFType *sampledType = nullptr) const;
        virtual Vector3 rho(const Vector3 &wo, int nSamples, const Vector2 *samples) const;
        virtual Vector3 rho(int nSamples, const Vector2 *samples1, const Vector2 *samples2) const;
        virtual double Pdf(const Vector3 &wo, const Vector3 &wi) const;
        virtual std::string ToString() const = 0;

        // BxDF Public Data
        const BxDFType type;
};


class BRDF: public BxDF{

};


class BTDF: public  BxDF{

};

class LambertianReflection : public BxDF{
    public:
        LambertianReflection(const Vector3 R): BxDF(BxDFType(BSDF_REFLECTION | BSDF_DIFFUSE)), R(R) {}
        Vector3 f (const Vector3 &wo, const Vector3 &wi) const;
        Vector3 rho(const Vector3 &, int, const Vector2 *)const {return R;}
        Vector3 rho(int, const Vector2 *, const Vector2 *) const {return R;}   
        std::string ToString() const;
    private:
        Vector3 R;
};

}

