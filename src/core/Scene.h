#ifndef SCENE_H
#define SCENE_H

#include "hikari.h"
#include "Ray.h"
#include "Vectors.h"
#include "Primitive.h"
#include "Light.h"
#include "shapes/Triangle.h"
#include "accelerators/BVH.h"
#include <vector>

namespace hikari {
    class Scene {
    public:
        Scene(){};
        void add(Shape *object);
        void addMesh(Mesh* mesh);
        bool intersectP(const Ray &ray) const;
        Intersection intersect(const Ray &ray)const;
        Vector3 castRay(const Ray &ray, int depth);
        std::vector<std::shared_ptr<Shape>> m_objects;
        BVHAccel *bvh;
        void buildBVH();

        std::vector<std::shared_ptr<Light> > lights;

    };


    void Scene::add(Shape *object) {
        m_objects.push_back( std::shared_ptr<Shape>(object) );
    }

    void Scene::addMesh(Mesh *mesh) {
        for(auto tri: mesh->tris){
            tri->m = & mesh->m_m;
            m_objects.push_back(std::shared_ptr<Triangle>(tri));
        }

        std::cout<<m_objects.size();
    }

    void Scene::buildBVH() {
        printf(" - Generating BVH...\n\n");
        this->bvh = new BVHAccel(m_objects, 4, BVHAccel::SplitMethod::SAH);
    }

    bool Scene::intersectP(const Ray &ray) const {
        return this->bvh->Intersect(ray);
    }
    Intersection Scene::intersect(const Ray &ray)const{
        return this->bvh->getIntersection(bvh->root, ray);
    }

//Randomly generate a direction in a hemisphere w.r.t shading normal n
    Vector3 getHemiRandomDirection(Vector3 n){
        double r1=2*M_PI*drand48(), r2=drand48(), r2s=sqrt(r2);
        Vector3 w=n;
        Vector3 u((fabs(w.x)>.1?Vector3(0,1):Vector3(1)).cross(w));
        u.normalize();
        Vector3 v = w.cross(u);
        return Vector3(u*cos(r1)*r2s + v*sin(r1)*r2s + w*sqrt(1-r2)).normalize();
    }
}

#endif //SCENE_H