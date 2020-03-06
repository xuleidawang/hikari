//
// Created by LEI XU on 2/27/20.
//

#ifndef PATHTRACER_INTEGRATOR_H
#define PATHTRACER_INTEGRATOR_H

#include "src/Vectors.h"
#include "Intersection.h"

namespace hikari{
    class Integrator {
    public:
        virtual  ~Integrator();
        virtual void Render(const Scene& scene) = 0;
    };

    Vector3 UniformSampleAllLights(const Scene &scene);
    Vector3 UniformSampleOneLight(const Scene &scene);

    class SamplerIntegrator : public Integrator {
    public:
        // SamplerIntegrator Public Methods
        SamplerIntegrator(std::shared_ptr<const Camera> camera,
                          std::shared_ptr<Sampler> sampler,
                          const Bounds2i &pixelBounds)
                : camera(camera), sampler(sampler), pixelBounds(pixelBounds) {}
        virtual void Preprocess(const Scene &scene, Sampler &sampler) {}
        void Render(const Scene &scene);
        virtual Vector3 Li(const Ray &ray, const Scene &scene,
                           Sampler &sampler, int depth = 0) const = 0;
//    Vector3 SpecularReflect(const Ray &ray,
//                             const Intersection &isect,
//                             const Scene &scene, Sampler &sampler, int depth) const;
//    Vector3 SpecularTransmit(const Ray &ray,
//                              const Intersection &isect,
//                              const Scene &scene, Sampler &sampler, int depth) const;

    protected:
        // SamplerIntegrator Protected Data
        std::shared_ptr<const Camera> camera;

    private:
        // SamplerIntegrator Private Data
        std::shared_ptr<Sampler> sampler;
        const Bounds2i pixelBounds;
    };
}
#endif //PATHTRACER_INTEGRATOR_H
