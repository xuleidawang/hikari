//
// Created by LEI XU on 2/29/20.
//

#ifndef PATHTRACER_WHITTED_H
#define PATHTRACER_WHITTED_H

#include "hikari.h"
#include "Integrator.h"
#include "Scene.h"
namespace hikari {
    // WhittedIntegrator Declarations
    class WhittedIntegrator : public SamplerIntegrator {
    public:
        // WhittedIntegrator Public Methods
        WhittedIntegrator(int maxDepth, std::shared_ptr<const Camera> camera, std::shared_ptr<Sampler> sampler, const Bounds2 &pixelBounds)
        : SamplerIntegrator(camera, sampler, pixelBounds), maxDepth(maxDepth) {}
        Vector3 Li(const Ray &ray, const Scene &scene,
                   Sampler &sampler, int depth) const;

    private:
        // WhittedIntegrator Private Data
        const int maxDepth;
    };

    WhittedIntegrator *CreateWhittedIntegrator(
            std::shared_ptr<Sampler> sampler,
            std::shared_ptr<const Camera> camera);


}


#endif //PATHTRACER_WHITTED_H
