#ifndef INTERSECTION_H
#define INTERSECTION_H


#include "hikari.h"
#include "Vectors.h"
#include "Ray.h"
#include "Material.h"


//enum Refl_t { DIFF, SPEC, REFR };  // material types, used in radiance() 
 namespace hikari{

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

         Ray SpawnRayTo(const Intersection &it) const {
             Vector3 origin = this->coords;
             Vector3 target = it.coords;
             Vector3 d = (target - origin).normalize();

             return Ray(origin, d);
         }


         void ComputeScatteringFunctions(
                 const Ray &ray,
                 bool allowMultipleLobes = false);

         bool happened;
         Vector3 coords;
         Vector3 normal;
         Vector3 wo;
         double distance;
         Shape* obj;
         Primitive *primitive;
         //Refl_t refl;
         Material* m;
     };
}

#endif 