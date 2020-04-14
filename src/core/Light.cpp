//
// Created by LEI XU on 3/3/20.
//

#include "Light.h"
#include "Scene.h"

namespace hikari {
    
    Light::Light(int nSamples)
            : nSamples(std::max(1, nSamples))
            {}
    Light::~Light() {}

    AreaLight::AreaLight(int nSamples)
            : Light(nSamples) {
        //++numAreaLights;
    }

    Vector3 AreaLight::L(const hikari::Intersection &intr, const hikari::Vector3 &w) const {
        return Vector3(0.0);
    }
}