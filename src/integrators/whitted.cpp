//
// Created by LEI XU on 2/29/20.
//

#include "integrators/whitted.h"
#include "Camera.h"
#include "Intersection.h"
#include "Scene.h"

namespace hikari {
    //Whitted integrator method definition
    Vector3 WhittedIntegrator::Li(const Ray &ray, const Scene &scene, Sampler &sampler, int depth) const
    {
        Vector3 L(0.,0.,0.);
        // Find closest ray intersection or return background radiance
        Intersection isect = scene.intersect(ray);
        if (!isect.happened) {
            for (const auto &light : scene.lights) L += light->Le(ray);
            return L;
        }

        // Compute emitted and reflected light at ray intersection point

        // Initialize common variables for Whitted integrator
        const Vector3 &n = isect.normal;
        Vector3 wo = isect.wo;

        // Compute scattering functions for surface interaction
        isect.ComputeScatteringFunctions(ray);
        if (!isect.brdf)
            return Li(isect.SpawnRay(ray.direction), scene, sampler, depth);

        // Compute emitted light if ray hit an area light source
        L += isect.Le(wo);

        // Add contribution of each light source
        for (const auto &light : scene.lights) {
            Vector3 wi;
            float pdf;
            VisibilityTester visibility;
            Vector3 Li =
                    light->Sample_Li(isect, sampler.Get2D(), &wi, &pdf, &visibility);
            if (Li.IsBlack() || pdf == 0) continue;
            Vector3 f = isect.brdf->f(wo, wi);
            //if (!f.IsBlack() && visibility.Unoccluded(scene))
            if(!f.IsBlack())
                L += f * Li * abs(wi.dot(n)) / pdf;
        }
        if (depth + 1 < maxDepth) {
            // Trace rays for specular reflection and refraction
            L += SpecularReflect(ray, isect, scene, sampler, depth);
            L += SpecularTransmit(ray, isect, scene, sampler, depth);
        }
        return L;
    }

}
