#ifndef SCENE_H
#define SCENE_H

#include "hikari.h"
#include "Ray.h"
#include "Vectors.h"
#include "Primitive.h"
#include "shapes/Triangle.h"
#include "accelerators/BVH.h"
#include <vector>

namespace hikari {
    class Object;
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


    Vector3  Scene::castRay(const Ray &ray, int depth){
        Vector3 resColor(0.0,0.0,0.0);
        // find the intersected object
        Intersection intersection = Scene::intersect(ray);

        if(!intersection.happened)return resColor;

        Material *m = intersection.m;
        Vector3 x = intersection.coords;
        Vector3 n=intersection.normal;
        Vector3 nl=n.dot(ray.direction)<0?n:n*-1;// hitting at outside or inside of the intersected object
        Vector3 f=m->getColor();

        double p = fmax(fmax(f.x,f.y),f.z);// max refl
        if (++depth>5)
        {
            if (drand48()<p)
                f=f*(1/p);
            else
                return m->getEmission(); //Russian Roulette
        }
        // Ideal DIFFUSE reflection
        if(m->getType() == DIFF)
        {
            Vector3 d = getHemiRandomDirection(nl);
            return m->getEmission() + f*castRay(Ray(x,d),depth);
        }
        else if(m->getType()==SPEC)
        {
            double roughness = 0.8;
            Vector3 reflected = ray.direction - n * 2 * n.dot(ray.direction);
            Vector3 refdirect (
                    reflected.x + (drand48()-0.5)*roughness,
                    reflected.y + (drand48()-0.5)*roughness,
                    reflected.z + (drand48()-0.5)*roughness
            );

            Ray reflRay(x,refdirect.normalize());
            return m->getEmission()+ f*castRay(reflRay, depth);
        }
        else if(m->getType() ==REFR)
        {
            double roughness = 0.8;
            Vector3 reflected = ray.direction - n * 2 * n.dot(ray.direction);
            Vector3 refdirect (
                    reflected.x + (drand48()-0.5)*roughness,
                    reflected.y + (drand48()-0.5)*roughness,
                    reflected.z + (drand48()-0.5)*roughness
            );
            Ray reflRay(x,refdirect.normalize());
            bool into = n.dot(nl)>0;                // Ray from outside going in?
            double nc=1, nt=1.5, nnt;
            if(into)
            {
                nnt = nc/nt;
            }
            else
            {
                nt = nt/nc;
            }

            double ddn=ray.direction.dot(nl);
            double cos2t = 1-nnt*nnt*(1-ddn*ddn);
            if(cos2t<0)
            {
                return  m->getEmission() + f*(castRay(reflRay,depth)); // Total internal reflection
            }
            Vector3 tdir = (ray.direction*nnt - n*((into?1:-1)*(ddn*nnt+sqrt(cos2t)))).normalize();
            double a=nt-nc, b=nt+nc, R0=a*a/(b*b), c = 1-(into?-ddn:tdir.dot(n));
            double Re=R0+(1-R0)*c*c*c*c*c,Tr=1-Re,P=.25+.5*Re,RP=Re/P,TP=Tr/(1-P);

            // Russian roulette
            if(depth > 2)
            {
                if( drand48() < P)
                {
                    return m->getEmission() + f * castRay(reflRay,depth)*RP;
                }
                else
                {
                    return m->getEmission() + f* castRay(Ray(x,tdir),depth)*TP;
                }
            }
            else
            {
                return m->getEmission() + f*castRay(reflRay,depth)*Re+castRay(Ray(x,tdir),depth)*Tr;
            }

        }
        return Vector3();
    }
}

#endif //SCENE_H