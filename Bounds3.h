#ifndef BOUNDS_H
#define BOUNDS_H

#include "Vector3.h"
#include <limits>
class Bounds3{
private:
	vector3 pMin,pMax; // two points to specify the bounding box 
public:
	Bounds3(){
		double minNum = std::numeric_limits<double>::lowest();
		double maxNum = std::nnumer_limits<double>::max();
		pMin = vector3(minNum,minNum,minNum);
		pMax = vector3(maxNum,maxNum,maxNum);
	}
	Bounds3(const Vector3 p):pMin(p), pMax(p){};
	Bounds3(const Vector3 p1, const vector3 p2){	
		pMin = Vector3(min(p1.x,p2.x),min(p1.y,p2.y),min(p1.z,p2.z));
		pMax = Vector3(max(p1.x,p2.x),max(p1.y,p2.y),max(p1.z,p2.z));};
	}

	Vector3 Diagonal()const {return pMax-pMin;}
	int maxExtent()const{
		Vector3 d = Diagonal();
		if(d.x>d.y && d.x>d.z)
			return 0;
		else if (d.y>d.z)
			return 1;
		else 
			return 2;
	}

	double SurfaceArea()const{Vector3 d = Diagonal();return 2*(d.x*d.y+d.x*d.z+d.y*d.z);}

	Vector3 Lerp(const Vector3 &t)const{
		return Vector3(Lerp(t.x,pMin.x,pMax.x),Lerp(t.y,pMin.y,pMax.y), Lerp(t.z,pMin.z,pMax.z));
	}

	Bounds3 Union(const Bounds3 &b){
		return Bounds3(Vector3(min(pMin.x,b.pMin.x),
							   min(pMin.y,b.pMin.y),
							   min(pMin.z,b.pMin.z)),
					   Vector3(max(pMax.x,b.pMax.x),
					   		   max(pMax.y,b.pMax.y),
					   		   max(pMax.z,b.pMax.z))
			);
	}

	Bounds3 Intersect(const Bounds3 &b){
		return Bounds3(Vector3(max(pMin.x,b.pMin.x),
							   max(pMin.y,b.pMin.y),
							   max(pMin.z,b.pMin.z)),
					   Vector3(min(pMax.x,b.pMax.x),
					   		   min(pMax.y,b.pMax.y),
					   		   min(pMax.z,b.pMax.z))
			);
	}

	bool Overlaps(const Bounds3<T> &b1, const Bounds3<T> &b2) {
		bool x = (b1.pMax.x >= b2.pMin.x) && (b1.pMin.x <= b2.pMax.x);
    	bool y = (b1.pMax.y >= b2.pMin.y) && (b1.pMin.y <= b2.pMax.y);
    	bool z = (b1.pMax.z >= b2.pMin.z) && (b1.pMin.z <= b2.pMax.z);
    	return (x && y && z);
    }

    bool Inside(const Vector3 &p, const Bounds3 &b) {
    	return (p.x >= b.pMin.x && p.x <= b.pMax.x &&
    		p.y >= b.pMin.y && p.y <= b.pMax.y &&
    		p.z >= b.pMin.z && p.z <= b.pMax.z);
    }
    inline const Vector3& operator[](int i) const {
    	return (i == 0) ? pMin : pMax;
    }



	inline bool IntersectP(const Ray &ray, float* hitt0, float* hitt1)const;
	inline bool IntersectP(const Ray &ray, const Vector3 &invDir, const int dirisNeg[3])const;
};


inline bool Bounds3::IntersectP(const Ray &ray, float* hitt0, float* hitt1)const{
	float t0 = 0, t1 = ray.t_max;
	for(int i=0;i<3;i++){
		float invRay = ray.direction_inv[i];
		float tNear = (pMin[i]-ray.origin[i])*invRay;
		float tFar = (pMax[i]-ray.origin[i])*invRay;
		if(tNear>tFar)swap(tNear,tFar);
		t0 = tNear > t0 ? tNear:t0;
		t1 = tFar  < t1 ? tFar :t1;
		if(t0>t1)return false;
	}

		if(hitt0) *hitt0 = t0;
		if(hitt1) *hitt1 = t1;
		return true;
}

inline bool Bounds3::IntersectP(const Ray &ray, const Vector3 &invDir, const int dirisNeg[3])const{
	const Bounds3f &bounds = *this;
    // Check for ray intersection against x and y slabs
    float  tMin = (bounds[dirIsNeg[0]].x - ray.origin.x) * invDir.x;
    float tMax = (bounds[1 - dirIsNeg[0]].x - ray.origin.x) * invDir.x;
    float tyMin = (bounds[dirIsNeg[1]].y - ray.origin.y) * invDir.y;
    float tyMax = (bounds[1 - dirIsNeg[1]].y - ray.origin.y) * invDir.y;

    // Update _tMax_ and _tyMax_ to ensure robust bounds intersection
    // tMax *= 1 + 2 * gamma(3);
    // tyMax *= 1 + 2 * gamma(3);
    if (tMin > tyMax || tyMin > tMax) return false;
    if (tyMin > tMin) tMin = tyMin;
    if (tyMax < tMax) tMax = tyMax;

    // Check for ray intersection against $z$ slab
    float tzMin = (bounds[dirIsNeg[2]].z - ray.o.z) * invDir.z;
    float tzMax = (bounds[1 - dirIsNeg[2]].z - ray.o.z) * invDir.z;

    // Update _tzMax_ to ensure robust bounds intersection
    // tzMax *= 1 + 2 * gamma(3);
    if (tMin > tzMax || tzMin > tMax) return false;
    if (tzMin > tMin) tMin = tzMin;
    if (tzMax < tMax) tMax = tzMax;
    return (tMin < ray.tMax) && (tMax > 0);
}










#endif //BOUNDS_H

