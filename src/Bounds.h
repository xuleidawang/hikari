#pragma once
#include "src/Vectors.h"
#include "src/core/Ray.h"
#include <limits>

namespace hikari {
    class Bounds{
    public:
        Vector3 pMin,pMax; // two points to specify the bounding box
        Bounds(){
            double minNum = std::numeric_limits<double>::lowest();
            double maxNum = std::numeric_limits<double>::max();
            pMax = Vector3(minNum,minNum,minNum);
            pMin = Vector3(maxNum,maxNum,maxNum);
        }
        Bounds(const Vector3 p):pMin(p), pMax(p){}
        Bounds(const Vector3 p1, const Vector3 p2){
            pMin = Vector3(fmin(p1.x,p2.x),fmin(p1.y,p2.y),fmin(p1.z,p2.z));
            pMax = Vector3(fmax(p1.x,p2.x),fmax(p1.y,p2.y),fmax(p1.z,p2.z));
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

        Bounds Intersect(const Bounds &b){
            return Bounds(Vector3(fmax(pMin.x,b.pMin.x),
                                   fmax(pMin.y,b.pMin.y),
                                   fmax(pMin.z,b.pMin.z)),
                           Vector3(fmin(pMax.x,b.pMax.x),
                                   fmin(pMax.y,b.pMax.y),
                                   fmin(pMax.z,b.pMax.z))
            );
        }

        Vector3 Offset(const Vector3 &p) const {
            Vector3 o = p - pMin;
            if (pMax.x > pMin.x) o.x /= pMax.x - pMin.x;
            if (pMax.y > pMin.y) o.y /= pMax.y - pMin.y;
            if (pMax.z > pMin.z) o.z /= pMax.z - pMin.z;
            return o;
        }

        bool Overlaps(const Bounds &b1, const Bounds &b2) {
            bool x = (b1.pMax.x >= b2.pMin.x) && (b1.pMin.x <= b2.pMax.x);
            bool y = (b1.pMax.y >= b2.pMin.y) && (b1.pMin.y <= b2.pMax.y);
            bool z = (b1.pMax.z >= b2.pMin.z) && (b1.pMin.z <= b2.pMax.z);
            return (x && y && z);
        }

        bool Inside(const Vector3 &p, const Bounds &b) {
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


    inline bool Bounds::IntersectP(const Ray &ray, float* hitt0, float* hitt1)const{
        float t0 = 0, t1 = ray.t_max;
        for(int i=0;i<3;i++){
            float invRay = ray.direction_inv[i];
            float tNear = (pMin[i]-ray.origin[i])*invRay;
            float tFar = (pMax[i]-ray.origin[i])*invRay;
            if(tNear>tFar)std::swap(tNear,tFar);
            t0 = tNear > t0 ? tNear:t0;
            t1 = tFar  < t1 ? tFar :t1;
            if(t0>t1)return false;
        }

        if(hitt0) *hitt0 = t0;
        if(hitt1) *hitt1 = t1;
        return true;
    }

    inline bool Bounds::IntersectP(const Ray &ray, const Vector3 &invDir, const int dirIsNeg[3])const{
        const Bounds &bounds = *this;
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
        float tzMin = (bounds[dirIsNeg[2]].z - ray.origin.z) * invDir.z;
        float tzMax = (bounds[1 - dirIsNeg[2]].z - ray.origin.z) * invDir.z;

        // Update _tzMax_ to ensure robust bounds intersection
        // tzMax *= 1 + 2 * gamma(3);
        if (tMin > tzMax || tzMin > tMax) return false;
        if (tzMin > tMin) tMin = tzMin;
        if (tzMax < tMax) tMax = tzMax;
        return (tMin < ray.t_max) && (tMax > 0);
    }



    inline Bounds Union(const Bounds &b1, const Bounds &b2) {
        Bounds ret;
        ret.pMin = Vector3::Min(b1.pMin, b2.pMin);
        ret.pMax = Vector3::Max(b1.pMax, b2.pMax);
        return ret;
    }

    inline Bounds Union(const Bounds &b,const Vector3 &p){
        Bounds ret;
        ret.pMin = Vector3::Min(b.pMin, p);
        ret.pMax = Vector3::Max(b.pMax, p);
        return ret;
    }

    class Bounds2{
    public:
        Vector2 pMin,pMax; // two points to specify the bounding box
        Bounds2(){
            double minNum = std::numeric_limits<double>::lowest();
            double maxNum = std::numeric_limits<double>::max();
            pMax = Vector2(minNum,minNum);
            pMin = Vector2(maxNum,maxNum);
        }
        Bounds2(const Vector2 p):pMin(p), pMax(p){}
        Bounds2(const Vector2 p1, const Vector2 p2){
            pMin = Vector2(fmin(p1.x,p2.x),fmin(p1.y,p2.y));
            pMax = Vector2(fmax(p1.x,p2.x),fmax(p1.y,p2.y));
        }

        Vector2 Diagonal()const {return pMax-pMin;}
        int maxExtent()const{
            Vector2 d = Diagonal();
            if(d.x>d.y)
                return 0;
            else
                return 1;
        }

        double SurfaceArea()const{Vector2 d = Diagonal();return 2*(d.x*d.y);}

        Bounds2 Intersect(const Bounds2 &b){
            return Bounds2(Vector2(fmax(pMin.x,b.pMin.x),
                                   fmax(pMin.y,b.pMin.y)
                                  ),
                           Vector2(fmin(pMax.x,b.pMax.x),
                                   fmin(pMax.y,b.pMax.y))
            );
        }

        Vector2 Offset(const Vector2 &p) const {
            Vector2 o = p - pMin;
            if (pMax.x > pMin.x) o.x /= pMax.x - pMin.x;
            if (pMax.y > pMin.y) o.y /= pMax.y - pMin.y;
            return o;
        }

        bool Overlaps(const Bounds2 &b1, const Bounds2 &b2) {
            bool x = (b1.pMax.x >= b2.pMin.x) && (b1.pMin.x <= b2.pMax.x);
            bool y = (b1.pMax.y >= b2.pMin.y) && (b1.pMin.y <= b2.pMax.y);

            return (x && y);
        }

        bool Inside(const Vector2 &p, const Bounds2 &b) {
            return (p.x >= b.pMin.x && p.x <= b.pMax.x &&
                    p.y >= b.pMin.y && p.y <= b.pMax.y);
        }
        inline const Vector2& operator[](int i) const {
            return (i == 0) ? pMin : pMax;
        }


        inline bool IntersectP(const Ray &ray, float* hitt0, float* hitt1)const;
        inline bool IntersectP(const Ray &ray, const Vector2 &invDir, const int dirisNeg[3])const;
    };







}


