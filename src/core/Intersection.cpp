//
// Created by LEI XU on 3/15/20.
//

#include "Intersection.h"

namespace hikari {

    Vector3 Intersection::Le(const Vector3 &w) const{
        const AreaLight *area = primitive->GetAreaLight();
        return area ? area->L(*this, w) : Vector3(0.f);
    }

    void Intersection::ComputeScatteringFunctions(const Ray &ray, bool allowMultipleLobes) {
        primitive->ComputeScatteringFunctions(this, allowMultipleLobes);
    }
}
