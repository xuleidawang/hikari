
#pragma once

#include "accelerators/BVH.h"
#include "shapes/Triangle.h"
#include <vector>

namespace hikari {
    class Scene {
    public:
        Scene(){};
        void addIntegrator(Integrator* integrator);
        void addSampler(Sampler* sampler);
        void addCamera(Camera* camera);

        void add(Shape *object, Material *material);
        void addMesh(Mesh* mesh, Material *material);
        void addPrimitive(Primitive *primitive);
        void addLight(Light *light);
        bool IntersectP(const Ray &ray) const;
        bool Intersect(const Ray &ray, Intersection* isect)const;
        Vector3 castRay(const Ray &ray, int depth);

        std::vector<std::shared_ptr<Primitive>> primitives;
        std::vector<std::shared_ptr<Light>> lights;

        BVHAccel *bvh;
        void buildBVH();

        std::vector<std::shared_ptr<Light> > lights;

        /// Return a pointer to the scene's integrator
        Integrator *getIntegrator() const { return integrator; }

        /// Return a pointer to the scene's camera
        Camera *getCamera() const { return camera; }

        /// Return a pointer to the scene's sample generator (const version)
        Sampler *getSampler() const { return sampler; }


    private:
        Integrator *integrator = nullptr;
        Sampler *sampler = nullptr;
        Camera *camera = nullptr;

    };
}
