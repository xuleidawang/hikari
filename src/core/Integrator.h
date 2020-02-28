//
// Created by LEI XU on 2/27/20.
//

#ifndef PATHTRACER_INTEGRATOR_H
#define PATHTRACER_INTEGRATOR_H

#include "src/Vectors.h"

class Integrator {
public:
    virtual  Inntegrator();
    virtual void Render(const Scene& scene) = 0;
};

Vector3 UniformSampleAllLights(const Scene &scene);
Vector3 UniformSampleOneLight(const Scene &scene);
#endif //PATHTRACER_INTEGRATOR_H
