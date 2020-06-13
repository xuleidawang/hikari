#pragma once
#include "Shape.h"
#include "math.h"

namespace hikari {
	class Sphere: public Shape{
	public:
		const double radius;
		const Vector3 pos;
		double zMin, zMax;
		//,e, color;
		//const Refl_t refl;
		Sphere():radius(0.0),pos(Vector3()){}


		Sphere(double rad_, Vector3 p_, Vector3 e_, Vector3 c_):
				radius(rad_), pos(p_){};

		//Inherited Virtual functions
		bool Intersect(const Ray& ray, Intersection* intersection)const;
		Bounds getBounds() const;
		float Area() const {return 4*PI*pow(radius,2);}

		Vector3 getNormal(Vector3 position);


	};
}
