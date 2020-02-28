#ifndef OBJECT_H
#define OBJECT_H

#include "Bounds3.h"
#include "src/core/Intersection.h"

class Shape {
public:
	virtual bool intersect(const Ray& ray, Intersection *intersection) = 0;
//	virtual Intersection getIntersection(Ray _ray) = 0;
//	virtual Vector3 getNormal(Vector3 position) = 0;
	virtual Bounds3 getBounds()=0;
	//Texture mapping
	//virtual bool getUVAt(Vector3, double*, double*) { return false; } 
	Vector3 m_p;

};
#endif

