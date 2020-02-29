#ifndef INTERSECTION_H
#define INTERSECTION_H


#include "src/Vectors.h"
#include "Material.h"
#include "Shape.h"
#include "Primitive.h"

//enum Refl_t { DIFF, SPEC, REFR };  // material types, used in radiance() 
 
class Shape;
class Primitive;
class Sphere;

struct Intersection
{
	Intersection(){
		happened=false;
		coords=Vector3();
		normal=Vector3();
		distance= std::numeric_limits<double>::max();
		obj =nullptr;
		m=nullptr;
	}
	bool happened;
	Vector3 coords;
	Vector3 normal;
	double distance;
	Shape* obj;
	Primitive *primitive;
	//Refl_t refl;
	Material* m;
};
#endif 