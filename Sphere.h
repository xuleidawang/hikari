#ifndef SPHERE_H
#define SPHERE_H
#include "Vectors.h"
#include "Object.h"
#include "Ray.h"
#include "Intersection.h"
#include "Material.h"

 
class Sphere : public Object {
public:
		const double radius;
		const Vector3 pos;
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
		bool intersect(const Ray& ray);
		Intersection getIntersection(Ray _ray);
		Vector3 getNormal(Vector3 position);

};



bool Sphere::intersect(const Ray& ray){
	double b = 2 * (ray.direction.x * (ray.origin.x - pos.x)+ ray.direction.y * (ray.origin.y - pos.y)+ ray.direction.z * (ray.origin.z - pos.z));
	double c = (ray.origin.x - pos.x) * (ray.origin.x - pos.x) +(ray.origin.y - pos.y) * (ray.origin.y - pos.y) +(ray.origin.z - pos.z) * (ray.origin.z - pos.z) -radius * radius;
	double d = b * b - 4 * c;
	//Negative delta means no intersections
	if (d < 0.0) return false;
	double t = (-b - sqrt(d)) * 0.5;
	if (t > EPSILON) return true;
	t = (-b + sqrt(d)) * 0.5;
	if (t > EPSILON) return true;
	return false;
}
Intersection Sphere::getIntersection(Ray _ray) {
	//Gets data about an intersection with a ray
	Intersection result;
	result.happened = false;

	double b = 2 * (_ray.direction.x * (_ray.origin.x - pos.x)
		+ _ray.direction.y * (_ray.origin.y - pos.y)
		+ _ray.direction.z * (_ray.origin.z - pos.z)
		);

	double c = (_ray.origin.x - pos.x) * (_ray.origin.x - pos.x) +
		(_ray.origin.y - pos.y) * (_ray.origin.y - pos.y) +
		(_ray.origin.z - pos.z) * (_ray.origin.z - pos.z) -
		radius * radius;

	double d = b * b - 4 * c;

	//Negative delta means no intersections
	if (d < 0) return result;

	double t = (-b - sqrt(d)) * 0.5;
	if (t > EPSILON) result.happened = true;
	else {
		t = (-b + sqrt(d)) * 0.5;
		if (t > EPSILON)
			result.happened = true;
		else
			return result;
	}

	result.coords = Vector3(_ray.origin + _ray.direction * t);
	result.normal = Vector3(result.coords - pos).normalize();

	result.obj = this;
	result.distance = t;

	return result;
}


Vector3 Sphere::getNormal(Vector3 position){
	Vector3 N = position-pos;
	return N.normalize();
}


#endif 
