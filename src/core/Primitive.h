//
// Created by LEI XU on 2/29/20.
//

#ifndef PATHTRACER_PRIMITIVE_H
#define PATHTRACER_PRIMITIVE_H

#include <memory>
#include "Shape.h"
#include "Material.h"
#include "Light.h"
#include "Intersection.h"

class Intersection;
class Shape;
class Primitive{
public:
    virtual ~Primitive();
    virtual Bounds3 WordldBound() const = 0;
    virtual bool Intersect(const Ray &ray, Intersection *) const =0;
    virtual bool IntersectP(const Ray &ray) const =0;
    //TODO Add AreaLight class
//    virtual const AreaLight *GetAreaLight() const = 0;
//    virtual const Material *GetMaterial() const = 0;
    virtual void ComputeScatteringFunctions(Intersection *isect,
                                            bool allowMultipleLobes) const = 0;
};

// GeometricPrimitive Declarations
class GeometricPrimitive : public Primitive {
public:
    // GeometricPrimitive Public Methods
    virtual Bounds3 WorldBound() const;
    virtual bool Intersect(const Ray &r, Intersection *isect) const;
    virtual bool IntersectP(const Ray &r) const;
    GeometricPrimitive(const std::shared_ptr<Shape> &shape,
                       const std::shared_ptr<Material> &material,
                       const std::shared_ptr<AreaLight> &areaLight,
                       const MediumInterface &mediumInterface);
    //const AreaLight *GetAreaLight() const;
    const Material *GetMaterial() const;
    void ComputeScatteringFunctions(Intersection *isect,
                                    bool allowMultipleLobes) const;

private:
    // GeometricPrimitive Private Data
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Material> material;
    //TODO light
//    std::shared_ptr<AreaLight> areaLight;
    //TODO Medium class
    MediumInterface mediumInterface;
};

#endif //PATHTRACER_PRIMITIVE_H
