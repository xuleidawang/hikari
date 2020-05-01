#pragma once

#include <vector>
#include "Shape.h"
#include "./lib/tiny_obj_loader/tiny_obj_loader.h"

namespace hikari {

    class mesh;
    class Triangle: public Shape {
    public:
        Vector3 v0, v1, v2; //vertices A, B ,C , counter-clockwise order
        Vector3 e1, e2; //2 edges v1-v0, v2-v0;
        Vector3 t0,t1,t2;//texture coords
        Vector3 normal;

        // Triangle Public Methods
        Triangle(Vector3 _v0, Vector3 _v1, Vector3 _v2):v0(_v0),v1(_v1),v2(_v2){
            e1 = v1-v0;
            e2 = v2-v0;
            normal = e1.cross(e2).normalize();
        }
        Triangle(Vector3 v0_, Vector3 v1_, Vector3 v2_, Vector3 t0_=Vector3(), Vector3 t1_=Vector3(), Vector3 t2_=Vector3(), Material *m_=NULL){
            v0=v0_, v1=v1_, v2=v2_, e1=v1-v0, e2=v2-v0, normal=e1.cross(e2).normalize();
            t0=t0_, t1=t1_, t2=t2_;
        }
        bool Intersect(const Ray& ray, Intersection* intersection)const;
        Vector3 getNormal(Vector3 position);
        Vector3 getBarycentric(Vector3 p);
        Bounds getBounds()const;
        Vector3 getMidpoint();
    };

    class Mesh : public Shape {

    public:
        std::vector<tinyobj::shape_t> m_shapes;
        std::vector<tinyobj::material_t> m_materials;
        // std::vector<Material> materials;

        //Material m_m;	// Material
        //BVH bvh;


        std::vector<Triangle*> tris;
        Mesh(Vector3 p_, const char* file_path);

        bool Intersect(const Ray& ray, Intersection* intersection)const;
        Vector3 getNormal(Vector3 position);
        Bounds getBounds()const;
    };


}

