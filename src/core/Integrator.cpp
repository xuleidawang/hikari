//
// Created by LEI XU on 3/18/20.
//
#include "integrator.h"
#include "scene.h"
#include "intersection.h"
#include "sampler.h"
#include "integrator.h"
#include "camera.h"

namespace hikari {
    // Integrator Method Definitions
    Integrator::~Integrator() {}
    Vector3 SamplerIntegrator::SpecularReflect(
            const Ray &ray, const Intersection &isect,
            const Scene &scene, Sampler &sampler, int depth) const {
        // Compute specular reflection direction _wi_ and BSDF value
        Vector3 wo = isect.wo, wi;
        float pdf;
        BxDFType type = BxDFType(BSDF_REFLECTION | BSDF_SPECULAR);
        Vector3 f = isect.brdf->Sample_f(wo, &wi, sampler.Get2D(), &pdf, type);

        // Return contribution of specular reflection
        const Vector3 &ns = isect.normal;
        if (pdf > 0.f && !f.IsBlack() && abs(wi.dot(ns)) != 0.f) {
            // Compute ray differential _rd_ for specular reflection
            Ray rd = isect.SpawnRay(wi);

            return f * Li(rd, scene, sampler, depth + 1) * abs(wi.dot(ns)) /
                   pdf;
        } else
            return Vector3(0.f);
    }

    Vector3 SamplerIntegrator::SpecularTransmit(
            const Ray &ray, const Intersection &isect,
            const Scene &scene, Sampler &sampler, int depth) const {
        Vector3 wo = isect.wo, wi;
        float pdf;
        const Vector3 &p = isect.coords;
        const Vector3 &ns = isect.normal;
        const BRDF &brdf = *isect.brdf;
        Vector3 f = brdf.Sample_f(wo, &wi, sampler.Get2D(), &pdf, BxDFType(BSDF_TRANSMISSION | BSDF_SPECULAR));
        Vector3 L = Vector3(0.f);
        if (pdf > 0.f && !f.IsBlack() && abs(wi.dot(ns)) != 0.f) {
            // Compute ray differential _rd_ for specular transmission
            Ray rd = isect.SpawnRay(wi);
            L = f * Li(rd, scene, sampler, depth + 1) * abs(wi.dot(ns)) / pdf;
        }
        return L;
    }


}