
#include "Light.h"
#include "Intersection.h"
#pragma once
namespace hikari
{
    class DiffuseAreaLight : public AreaLight{
        public:
        DiffuseAreaLight(const Vector3 &Lemit, int nSamples, const std::shared_ptr<Shape> &shape, bool twoSided = false);
        Vector3 L (const Intersection &intr, const Vector3 &w) const {
            return (twoSided || intr.normal.dot(w) > 0) ? Lemit : Vector3(0.0);
            }

        Vector3 Power() const;
        Vector3 Sample_Li(const Intersection &ref, const Vector2 &u, Vector3 *wo, float *pdf, VisibilityTester *vis) const;
        float Pdf_Li(const Intersection &, const Vector3 &) const;
        Vector3 Sample_Le(const Vector2 &u1, const Vector2 &u2, float time, Ray *ray, Vector3 *nLight, float *pdfPos, float *pdfDir) const;
        void Pdf_Le(const Ray &, const Vector3 &, float *pdfPos, float *pdfDir) const;

protected:
const Vector3 Lemit;
std::shared_ptr<Shape> shape;
const bool twoSided;
const float area;

    };
} // namespace hikari

