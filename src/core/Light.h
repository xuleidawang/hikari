//
// Created by LEI XU on 3/3/20.
//

#pragma once

#include "Intersection.h"

namespace hikari {
    class Light{
    public:
        virtual ~Light();
        Light(int nSamples = 1);
        virtual Vector3 Le(const Ray &r) const;
        virtual Vector3 Sample_Li(const Intersection &ref, const Vector2 &u, Vector3 *wi, float *pdf, VisibilityTester *vis)const =0;
        const int nSamples;
    };

    // A diffuse AreaLight
    class AreaLight: public Light {
    public:
        AreaLight(int nSamples);
        Vector3 L(const Intersection &intr, const Vector3 &w) const;
        Vector3 Sample_Li(const Intersection &ref, const Vector2 &u, Vector3 *wi, float *pdf, VisibilityTester *vis)const override;
    };

    class VisibilityTester{
    public:
        VisibilityTester(){}
        Intersection *p0, *p1;
    };

}
