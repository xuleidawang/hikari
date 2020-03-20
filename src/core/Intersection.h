#ifndef INTERSECTION_H
#define INTERSECTION_H


#include "hikari.h"
#include "Vectors.h"
#include "Ray.h"
#include "Material.h"
#include "Primitive.h"


//enum Refl_t { DIFF, SPEC, REFR };  // material types, used in radiance() 
 namespace hikari{

     struct Intersection
     {
         Intersection(){
             happened=false;
             coords=Vector3();
             normal=Vector3();
             distance= std::numeric_limits<double>::max();
             m=nullptr;
         }

         Ray SpawnRay(const Vector3 &d) const {
             return Ray(this->coords, d);
         }

         Ray SpawnRayTo(const Intersection &it)  {
             Vector3 origin = this->coords;
             Vector3 target = it.coords;
             Vector3 d = (target - origin).normalize();

             return Ray(origin, d);
         }
         Vector3 Le(const Vector3 &w) const;
         void ComputeScatteringFunctions(const Ray &ray, bool allowMultipleLobes = false);

         bool happened;
         Vector3 coords;
         Vector3 normal;
         Vector3 wo;
         double distance;
         Shape *shape = nullptr;
         Primitive *primitive = nullptr;
         BRDF *brdf;
         Material* m;
     };
}

#endif 