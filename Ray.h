#ifndef RAY_H
#define RAY_H
#include "Vectors.h"

struct Ray{
	//Destination = origin + t*direction
	Vector3 origin;
	Vector3 direction, direction_inv;
	double t;//transportation time,

	Ray(const Vector3& ori, const Vector3& dir, const double _t = 0.0): origin(ori), direction(dir),t(_t) {
		direction_inv = Vector3(1./direction.x, 1./direction.y, 1./direction.z);
	}

	Vector3 operator()(double t) const{return origin+direction*t;}
	
	friend std::ostream &operator<<(std::ostream& os, const Ray& r){
		os<<"[origin:="<<r.origin<<", direction="<<r.direction<<", time="<< r.t<<"]\n";
		return os;
	}
};


#endif

