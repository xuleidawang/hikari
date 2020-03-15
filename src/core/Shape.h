#ifndef OBJECT_H
#define OBJECT_H

#include "Bounds.h"
#include "Intersection.h"

namespace hikari {
    class Intersection;

    class Shape {
    public:
        virtual bool intersect(const Ray& ray, Intersection *intersection);
//	virtual Intersection getIntersection(Ray _ray) = 0;
//	virtual Vector3 getNormal(Vector3 position) = 0;
        virtual Bounds getBounds()const =0;
        //Texture mapping
        //virtual bool getUVAt(Vector3, double*, double*) { return false; }
        //virtual Bounds WorldBound() const;
    };
}

#endif

