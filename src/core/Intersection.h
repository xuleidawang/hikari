#pragma once

#include "Vectors.h"
#include "Ray.h"
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
         const Shape *shape = nullptr;
         const Primitive *primitive = nullptr;
         BSDF *bsdf;
     };
}
