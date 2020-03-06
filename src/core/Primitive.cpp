//
// Created by LEI XU on 2/29/20.
//

#include "Primitive.h"

namespace hikari {
    Primitive::~Primitive() {}

    Bounds3 GeometricPrimitive::WorldBound() const { return shape->WorldBound(); }
//bool GeometricPrimitive::IntersectP(const Ray &r) const {
//    return shape->IntersectP(r);
//}

    bool GeometricPrimitive::Intersect(const Ray &r,
                                       Intersection *isect) const {
        float tHit;
        if (!shape->intersect(r, isect)) return false;
        //r.t_max = tHit;
        isect->primitive = this;
//    CHECK_GE(Dot(isect->, isect->shading.n), 0.);
        // Initialize _SurfaceInteraction::mediumInterface_ after _Shape_
        // intersection
//    if (mediumInterface.IsMediumTransition())
//        isect->mediumInterface = mediumInterface;
//    else
//        isect->mediumInterface = MediumInterface(r.medium);
        return true;
    }

    const AreaLight *GeometricPrimitive::GetAreaLight() const {
        return areaLight.get();
    }

    const Material *GeometricPrimitive::GetMaterial() const {
        return material.get();
    }

    void GeometricPrimitive::ComputeScatteringFunctions(
            Intersection *isect, TransportMode mode,
            bool allowMultipleLobes) const {
        if (material)
            material->ComputeScatteringFunctions(isect, arena, mode,
                                                 allowMultipleLobes);
        CHECK_GE(Dot(isect->n, isect->shading.n), 0.);
    }
}
