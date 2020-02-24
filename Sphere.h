#ifndef SPHERE_H
#define SPHERE_H

#include "Object.h"
#include "Intersection.h"
#include "Vectors.h"
#include "Ray.h"
#include "Material.h"
#include "Bounds3.h"


 
class Sphere: public Object{
public:
		const double radius;
		const Vector3 pos;
		double zMin, zMax;
		//,e, color;
		Material material;
		//const Refl_t refl;
		Sphere():radius(0.0),pos(Vector3()),material(Material()){}
		Sphere(double rad_, Vector3 p_, Material m):radius(rad_),pos(p_), material(m){};

		Sphere(double rad_, Vector3 p_, Vector3 e_, Vector3 c_, MaterialType refl_):
		radius(rad_), pos(p_){
			this->material = Material(refl_, c_, e_);
		} 

		//Inherited Virtual functions
		bool intersect(const Ray& ray, Intersection* intersection);
		Intersection getIntersection(Ray _ray);
		Vector3 getNormal(Vector3 position);
		Bounds3 getBounds();

};



#endif
