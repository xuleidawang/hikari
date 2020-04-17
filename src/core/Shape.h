#ifndef SHAPE_H
#define SHAPE_H

#include "Bounds.h"
#include "Intersection.h"

namespace hikari {
    class Intersection;

    class Shape {
    public:
        virtual bool intersect(const Ray& ray, Intersection *intersection);
        virtual Bounds getBounds() const =0;
    };
}

#endif

