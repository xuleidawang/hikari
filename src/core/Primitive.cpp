//
// Created by LEI XU on 2/29/20.
//

#include "Primitive.h"
namespace hikari {
    Primitive::~Primitive() {}

////bool GeometricPrimitive::IntersectP(const Ray &r) const {
////    return shape->IntersectP(r);
////}
//
//    bool GeometricPrimitive::Intersect(const Ray &r,
//                                       Intersection *isect) const {
//        float tHit;
//        if (!shape->intersect(r, isect)) return false;
//        //r.t_max = tHit;
//        isect->primitive = this;
////    CHECK_GE(Dot(isect->, isect->shading.n), 0.);
//        // Initialize _SurfaceInteraction::mediumInterface_ after _Shape_
//        // intersection
////    if (mediumInterface.IsMediumTransition())
////        isect->mediumInterface = mediumInterface;
////    else
////        isect->mediumInterface = MediumInterface(r.medium);
//        return true;
//    }
//

    void GeometricPrimitive::ComputeScatteringFunctions( Intersection *isect, bool allowMultipleLobes) const {
        if (material)
            material->ComputeScatteringFunctions(isect, allowMultipleLobes);
//        CHECK_GE(Dot(isect->n, isect->normal), 0.);
}
    Bounds GeometricPrimitive::WorldBound() const { return shape->getBounds(); }

    bool GeometricPrimitive::Intersect(const hikari::Ray &ray, hikari::Intersection *isect) const {
        return shape->intersect(ray, isect);
    }

    const AreaLight *GeometricPrimitive::GetAreaLight() const{ return this->areaLight.get(); }
    const Material *GeometricPrimitive::GetMaterial() const{ return material.get();}


}

