#ifndef INTERSECTION_H
#define INTERSECTION_H
#include "Sphere.h"
#include "Vectors.h"
#include "Material.h"
#include "Object.h"
//enum Refl_t { DIFF, SPEC, REFR };  // material types, used in radiance() 
 
class Object;
class Sphere;

struct Intersection
{
	bool happened;
	Vector3 coords;
	Vector3 normal;
	double distance;
	Object* obj;
	//Refl_t refl;
	Material m;
};

// // SurfaceInteraction Declarations
// class SurfaceInteraction : public Intersection {
//   public:
//     // SurfaceInteraction Public Methods
//     SurfaceInteraction() {}
//     SurfaceInteraction(const Point3f &p, const Vector3f &pError,
//                        const Point2f &uv, const Vector3f &wo,
//                        const Vector3f &dpdu, const Vector3f &dpdv,
//                        const Normal3f &dndu, const Normal3f &dndv, Float time,
//                        const Shape *sh,
//                        int faceIndex = 0);
//     void SetShadingGeometry(const Vector3f &dpdu, const Vector3f &dpdv,
//                             const Normal3f &dndu, const Normal3f &dndv,
//                             bool orientationIsAuthoritative);
//     void ComputeScatteringFunctions(
//         const RayDifferential &ray, MemoryArena &arena,
//         bool allowMultipleLobes = false,
//         TransportMode mode = TransportMode::Radiance);
//     void ComputeDifferentials(const RayDifferential &r) const;
//     Spectrum Le(const Vector3f &w) const;

//     // SurfaceInteraction Public Data
//     Point2f uv;
//     Vector3f dpdu, dpdv;
//     Normal3f dndu, dndv;
//     const Shape *shape = nullptr;
//     struct {
//         Normal3f n;
//         Vector3f dpdu, dpdv;
//         Normal3f dndu, dndv;
//     } shading;
//     const Primitive *primitive = nullptr;
//     BSDF *bsdf = nullptr;
//     BSSRDF *bssrdf = nullptr;
//     mutable Vector3f dpdx, dpdy;
//     mutable Float dudx = 0, dvdx = 0, dudy = 0, dvdy = 0;

//     // Added after book publication. Shapes can optionally provide a face
//     // index with an intersection point for use in Ptex texture lookups.
//     // If Ptex isn't being used, then this value is ignored.
//     int faceIndex = 0;
// };

// } 


#endif 