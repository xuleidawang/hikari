//
// Created by LEI XU on 3/15/20.
//

#include "Intersection.h"

namespace hikari {

    void Intersection::ComputeScatteringFunctions(const Ray &ray,
                                                 bool allowMultipleLobes) {
        primitive->ComputeScatteringFunctions(this, arena, mode,
                                              allowMultipleLobes);
    }

    Vector3 Intersection::Le(const Vector3 &w) const {
        const AreaLight *area = primitive->GetAreaLight();
        return area ? area->L(*this, w) : Spectrum(0.f);
    }
}
