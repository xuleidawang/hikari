//
// Created by LEI XU on 12/3/18.
//
#include "Reflection.h"

namespace hikari {
    // BSDF Method Definitions
Vector3 BSDF::f(const Vector3 &woW, const Vector3 &wiW,
                 BxDFType flags) const {
    // ProfilePhase pp(Prof::BSDFEvaluation);
    // Vector3 wi = WorldToLocal(wiW), wo = WorldToLocal(woW);
    Vector3 wi = wiW, wo = woW;
    if (wo.z == 0) return 0.;
    // bool reflect = Dot(wiW, ng) * Dot(woW, ng) > 0;
    bool reflect = wiW.dot(ng) * woW.dot(ng) >0;
    Vector3 f(0.f);
    for (int i = 0; i < nBxDFs; ++i)
        if (bxdfs[i]->MatchesFlags(flags) &&
            ((reflect && (bxdfs[i]->type & BSDF_REFLECTION)) ||
             (!reflect && (bxdfs[i]->type & BSDF_TRANSMISSION))))
            f += bxdfs[i]->f(wo, wi);
    return f;
}

Vector3 BSDF::rho(int nSamples, const Vector2 *samples1,
                   const Vector2 *samples2, BxDFType flags) const {
    Vector3 ret(0.f);
    for (int i = 0; i < nBxDFs; ++i)
        if (bxdfs[i]->MatchesFlags(flags))
        ret += bxdfs[i]->rho(nSamples, samples1, samples2);
    return ret;
}

Vector3 BSDF::rho(const Vector3 &wo, int nSamples, const Vector2 *samples,
                   BxDFType flags) const {
    Vector3 ret(0.f);
    for (int i = 0; i < nBxDFs; ++i)
        if (bxdfs[i]->MatchesFlags(flags))
            ret += bxdfs[i]->rho(wo, nSamples, samples);
    return ret;
}

Vector3 BSDF::Sample_f(const Vector3 &woWorld, Vector3 *wiWorld,
                        const Vector2 &u, float *pdf, BxDFType type,
                        BxDFType *sampledType) const {
    // ProfilePhase pp(Prof::BSDFSampling);
    // Choose which _BxDF_ to sample
    int matchingComps = NumComponents(type);
    if (matchingComps == 0) {
        *pdf = 0;
        if (sampledType) *sampledType = BxDFType(0);
        return Vector3(0.0);
    }
    int comp =
        std::min((int)std::floor(u[0] * matchingComps), matchingComps - 1);

    // Get _BxDF_ pointer for chosen component
    BxDF *bxdf = nullptr;
    int count = comp;
    for (int i = 0; i < nBxDFs; ++i)
        if (bxdfs[i]->MatchesFlags(type) && count-- == 0) {
            bxdf = bxdfs[i];
            break;
        }
    // CHECK(bxdf != nullptr);
    // VLOG(2) << "BSDF::Sample_f chose comp = " << comp << " / matching = " <<
    //     matchingComps << ", bxdf: " << bxdf->ToString();

    // Remap _BxDF_ sample _u_ to $[0,1)^2$
    Vector2 uRemapped(std::min(u[0] * matchingComps - comp, OneMinusEpsilon), u[1]);

    // Sample chosen _BxDF_
    Vector3 wi, wo = (woWorld);
    if (wo.z == 0) return 0.;
    *pdf = 0;
    if (sampledType) *sampledType = bxdf->type;
    Vector3 f = bxdf->Sample_f(wo, &wi, uRemapped, pdf, sampledType);
    // VLOG(2) << "For wo = " << wo << ", sampled f = " << f << ", pdf = "
    //         << *pdf << ", ratio = " << ((*pdf > 0) ? (f / *pdf) : Spectrum(0.))
    //         << ", wi = " << wi;
    if (*pdf == 0) {
        if (sampledType) *sampledType = BxDFType(0);
        return 0;
    }

    // Compute overall PDF with all matching _BxDF_s
    if (!(bxdf->type & BSDF_SPECULAR) && matchingComps > 1)
        for (int i = 0; i < nBxDFs; ++i)
            if (bxdfs[i] != bxdf && bxdfs[i]->MatchesFlags(type))
                *pdf += bxdfs[i]->Pdf(wo, wi);
    if (matchingComps > 1) *pdf /= matchingComps;

    // Compute value of BSDF for sampled direction
    if (!(bxdf->type & BSDF_SPECULAR)) {
        // bool reflect = Dot(*wiWorld, ng) * Dot(woWorld, ng) > 0;
        bool reflect = (wiWorld->dot(ng))*(woWorld.dot(ng)) > 0 ;
        f = 0.;
        for (int i = 0; i < nBxDFs; ++i)
            if (bxdfs[i]->MatchesFlags(type) &&
                ((reflect && (bxdfs[i]->type & BSDF_REFLECTION)) ||
                 (!reflect && (bxdfs[i]->type & BSDF_TRANSMISSION))))
                f += bxdfs[i]->f(wo, wi);
    }
    // VLOG(2) << "Overall f = " << f << ", pdf = " << *pdf << ", ratio = "
    //         << ((*pdf > 0) ? (f / *pdf) : Spectrum(0.));
    return f;
}

float BSDF::Pdf(const Vector3 &woWorld, const Vector3 &wiWorld,
                BxDFType flags) const {
    // ProfilePhase pp(Prof::BSDFPdf);
    if (nBxDFs == 0.f) return 0.f;
    Vector3 wo = (woWorld), wi = (wiWorld);
    if (wo.z == 0) return 0.;
    float pdf = 0.f;
    int matchingComps = 0;
    for (int i = 0; i < nBxDFs; ++i)
        if (bxdfs[i]->MatchesFlags(flags)) {
            ++matchingComps;
            pdf += bxdfs[i]->Pdf(wo, wi);
        }
    float v = matchingComps > 0 ? pdf / matchingComps : 0.f;
    return v;
}

std::string BSDF::ToString() const {
    // std::string s = StringPrintf("[ BSDF eta: %f nBxDFs: %d", eta, nBxDFs);
    // for (int i = 0; i < nBxDFs; ++i)
    //     s += StringPrintf("\n  bxdfs[%d]: ", i) + bxdfs[i]->ToString();
    return  std::string(" []");
}

inline int BSDF::NumComponents(BxDFType flags) const {
    int num = 0;
    for (int i=0; i < nBxDFs; i++)
        if(bxdfs[i] -> MatchesFlags(flags)) num++;
    return num;
}

Vector3 BxDF::Sample_f(const Vector3 &wo, Vector3 *wi, const Vector2 &u, float *pdf, BxDFType *sampledType) const 
{
    *wi = CosineSampleHemisphere (u);
    if(wo.z < 0)
        wi->z *= -1;
    *pdf = Pdf(wo, *wi);
    return f(wo, *wi);
}

float BxDF::Pdf(const Vector3 &wo, const Vector3 &wi) const {
    return SameHemisphere(wi, wo)? AbsCosTheta(wi)/PI :0;
}

Vector3 BxDF::rho(const Vector3 &w, int nSamples, const Vector2 *u) const{
    Vector3 r(0.0);
    for (int i = 0; i < nSamples; i++)
    {
        Vector3 wi;
        float pdf = 0;
        Vector3 f = Sample_f(w, &wi, u[i], &pdf);
        if(pdf > 0) 
        r += f*AbsCosTheta(wi) / pdf;
    }
    return r / nSamples;
}

Vector3 BxDF::rho(int nSamples, const Vector2 *u1, const Vector2 *u2) const{
    Vector3 r(0.f);
    for (int i = 0; i < nSamples; ++i) {
        // Estimate one term of $\rho_\roman{hh}$
        Vector3 wo, wi;
        wo = UniformSampleHemisphere(u1[i]);
        float pdfo = UniformHemispherePdf(), pdfi = 0;
        Vector3 f = Sample_f(wo, &wi, u2[i], &pdfi);
        if (pdfi > 0)
            r += f * AbsCosTheta(wi) * AbsCosTheta(wo) / (pdfo * pdfi);
    }
    return r / (PI * nSamples);
}

Vector3 LambertianReflection::f(const Vector3 &wo, const Vector3 &wi) const{
    return R /M_PI;
}

std::string LambertianReflection::ToString() const {
    return std::string("LambertianReflection R");
}





}
