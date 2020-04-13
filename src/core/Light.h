//
// Created by LEI XU on 3/3/20.
//

#ifndef PATHTRACER_LIGHT_H
#define PATHTRACER_LIGHT_H

#include "hikari.h"
#include "Intersection.h"

namespace hikari {
    class Light{
    public:
        virtual ~Light();
        Light(int nSamples = 1);
        virtual Vector3 Le(const Ray &r) const;
        virtual Vector3 Sample_Li(const Intersection &ref, const Vector2 &u, Vector3 *wi, float *pdf, VisibilityTester *vis) const =0;
//        virtual Vector3 Power() const = 0;
//        virtual double Pdf_Li(const Intersection &ref, const Vector3 &wi) const = 0;
//        virtual Vector3 Sample_Le(const Vector2 &u1, const Vector2 &u2, float time,
//                                   Ray *ray, Vector3 *nLight, float *pdfPos,
//                                  float *pdfDir) const = 0;
//        virtual void Pdf_Le(const Ray &ray, const Vector3 &nLight, float *pdfPos,
//                            float *pdfDir) const = 0;

        const int nSamples;
    };

    class AreaLight: public Light {

    public:
        AreaLight(int nSamples);
//        Vector3 L(const Intersection &intr, const Vector3 &w) const = 0;
    };

    class VisibilityTester{
    public:
        VisibilityTester(){}
        Intersection *p0, *p1;
    };

}

#endif //PATHTRACER_LIGHT_H