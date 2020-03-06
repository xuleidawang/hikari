//
// Created by LEI XU on 3/3/20.
//

#include "Light.h"

namespace hikari {
    
    Light::Light(int nSamples)
            : nSamples(std::max(1, nSamples))
            {}
    Light::~Light() {}

    bool VisibilityTester::Unoccluded(const Scene &scene) const {
        return !scene.intersectP(p0.SpawnRayTo(p1));
    }
    AreaLight::AreaLight(int nSamples)
            : Light(nSamples) {
        //++numAreaLights;
    }
}