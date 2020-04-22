//
// Created by LEI XU on 2/29/20.
//

#ifndef PATHTRACER_PRIMITIVE_H
#define PATHTRACER_PRIMITIVE_H

#include "hikari.h"
#include "Shape.h"
#include "Material.h"
#include "Light.h"

namespace  hikari {
    class Intersection;

    class Shape;

    class Primitive {
    public:
        virtual ~Primitive();

        virtual Bounds WorldBound() const = 0;

        virtual bool Intersect(const Ray &ray, Intersection *) const = 0;

        virtual const AreaLight *GetAreaLight() const = 0;
        virtual const Material *GetMaterial() const = 0;
        virtual void ComputeScatteringFunctions(Intersection *isect, bool allowMultipleLobes) const =0;
    };

// GeometricPrimitive Declarations
    class GeometricPrimitive : public Primitive {
    public:
        // GeometricPrimitive Public Methods
        Bounds WorldBound() const;
        bool Intersect(const Ray &r, Intersection *isect) const;
        GeometricPrimitive(const std::shared_ptr<Shape> &shape,
                           const std::shared_ptr<Material> &material,
                           const std::shared_ptr<AreaLight> &areaLight
                           );
        GeometricPrimitive(const std::shared_ptr<Shape> &shape,
                           const std::shared_ptr<Material> &material
                           );
        const AreaLight *GetAreaLight() const;
        const Material *GetMaterial() const;
        void ComputeScatteringFunctions(Intersection *isect,
                                        bool allowMultipleLobes) const;

    private:
        // GeometricPrimitive Private Data
        std::shared_ptr<Shape> shape;
        std::shared_ptr<Material> material;
        std::shared_ptr<AreaLight> areaLight;
        //TODO Medium class
//        MediumInterface mediumInterface;
    };
}


#endif //PATHTRACER_PRIMITIVE_H
