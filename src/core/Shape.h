#ifndef SHAPE_H
#define SHAPE_H

#include "Bounds.h"
#include "Intersection.h"

namespace hikari {
    class Intersection;

    class Shape {
    public:
        virtual bool Intersect(const Ray& ray, Intersection *intersection)const =0;
        virtual Bounds getBounds() const =0;
    };
}

#endif

