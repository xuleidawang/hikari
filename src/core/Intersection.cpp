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
}