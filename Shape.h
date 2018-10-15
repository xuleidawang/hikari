#include "Intersection.h"

class Shape{
public:
	virtual bool intersect(const Ray& ray) = 0;
	virtual Intersection getIntersection(Ray _ray) = 0;
	virtual Vector3 getNormal(Vector3 position) = 0;
	//Texture mapping
	//virtual bool getUVAt(Vector3, double*, double*) { return false; } 

};
