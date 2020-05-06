
#pragma once

#include "accelerators/BVH.h"
#include "shapes/Triangle.h"
#include <vector>

namespace hikari {
    class Scene {
    public:
        Scene(){};
        void add(Shape *object, Material *material);
        void addMesh(Mesh* mesh, Material *material);
        void addPrimitive(Primitive *primitive);
        bool IntersectP(const Ray &ray) const;
        bool Intersect(const Ray &ray, Intersection* isect)const;
        Vector3 castRay(const Ray &ray, int depth);
        std::vector<std::shared_ptr<Primitive>> primitives;
        BVHAccel *bvh;
        void buildBVH();

        std::vector<std::shared_ptr<Light> > lights;

        /// Return a pointer to the scene's integrator
        Integrator *getIntegrator() { return m_integrator; }

        /// Return a pointer to the scene's camera
        const Camera *getCamera() const { return m_camera; }

        /// Return a pointer to the scene's sample generator (const version)
        const Sampler *getSampler() const { return m_sampler; }


    private:
        Integrator *m_integrator = nullptr;
        Sampler *m_sampler = nullptr;
        Camera *m_camera = nullptr;

    };
}
