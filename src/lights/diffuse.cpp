#include "lights/diffuse.h"
#include "shapes/triangle.h"

namespace hikari {

// DiffuseAreaLight Method Definitions
DiffuseAreaLight::DiffuseAreaLight( const Vector3 &Lemit, int nSamples,
                                   const std::shared_ptr<Shape> &shape,
                                   bool twoSided)
    : AreaLight(nSamples),
      Lemit(Lemit),
      shape(shape),
      twoSided(twoSided),
      area(shape->Area()) {}

Vector3 DiffuseAreaLight::Power() const {
    return (twoSided ? 2 : 1) * Lemit * area * Pi;
}

Vector3 DiffuseAreaLight::Sample_Li(const Intersection &ref, const Vector2 &u,
                                     Vector3 *wi, float *pdf,
                                     VisibilityTester *vis) const {
    Intersection pShape = shape->Sample(ref, u, pdf);
    if (*pdf == 0 || (pShape.p - ref.p).LengthSquared() == 0) {
        *pdf = 0;
        return 0.f;
    }
    *wi = (pShape.p - ref.p).normalized();
    *vis = VisibilityTester(ref, pShape);
    return L(pShape, -*wi);
}

float DiffuseAreaLight::Pdf_Li(const Interaction &ref,
                               const Vector3f &wi) const {
    // ProfilePhase _(Prof::LightPdf);
    return shape->Pdf(ref, wi);
}

Vector3 DiffuseAreaLight::Sample_Le(const Vector2 &u1, const Vector2 &u2,
                                     float time, Ray *ray, Vector3 *nLight,
                                     float *pdfPos, float *pdfDir) const {
    // Sample a point on the area light's _Shape_, _pShape_
    Intersection pShape = shape->Sample(u1, pdfPos);
    // pShape.mediumInterface = mediumInterface;
    *nLight = pShape.n;

    // Sample a cosine-weighted outgoing direction _w_ for area light
    Vector3 w;
    if (twoSided) {
        Vector2 u = u2;
        // Choose a side to sample and then remap u[0] to [0,1] before
        // applying cosine-weighted hemisphere sampling for the chosen side.
        if (u[0] < .5) {
            u[0] = std::min(u[0] * 2, OneMinusEpsilon);
            w = CosineSampleHemisphere(u);
        } else {
            u[0] = std::min((u[0] - .5f) * 2, OneMinusEpsilon);
            w = CosineSampleHemisphere(u);
            w.z *= -1;
        }
        *pdfDir = 0.5f * CosineHemispherePdf(std::abs(w.z));
    } else {
        w = CosineSampleHemisphere(u2);
        *pdfDir = CosineHemispherePdf(w.z);
    }

    Vector3 v1, v2, n(pShape.n);
    CoordinateSystem(n, &v1, &v2);
    w = w.x * v1 + w.y * v2 + w.z * n;
    *ray = pShape.SpawnRay(w);
    return L(pShape, w);
}

void DiffuseAreaLight::Pdf_Le(const Ray &ray, const Normal3f &n, Float *pdfPos,
                              Float *pdfDir) const {
    // ProfilePhase _(Prof::LightPdf);
    Intersection it(ray.o, n, Vector3f(), Vector3f(n), ray.time,
                   mediumInterface);
    *pdfPos = shape->Pdf(it);
    *pdfDir = twoSided ? (.5 * CosineHemispherePdf(AbsDot(n, ray.d)))
                       : CosineHemispherePdf(Dot(n, ray.d));
}

}  // namespace pbrt