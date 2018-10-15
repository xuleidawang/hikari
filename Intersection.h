#ifndef INTERSECTION_H
#define INTERSECTION_H
#include "Sphere.h"
#include "Vectors.h"

enum Refl_t { DIFF, SPEC, REFR };  // material types, used in radiance() 
 
class Shape;
class Sphere;

struct Intersection
{
	bool happened;
	Vector3 coords;
	Vector3 normal;
	double distance;
	Shape* obj;
	Refl_t refl;
};


#endif 