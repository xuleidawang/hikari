#include "Scene.h"

namespace hikari
{

    void Scene::addIntegrator(Integrator *integrator)
    {
        this->integrator = integrator;
    }
    void Scene::addCamera(Camera *camera)
    {
        this->camera = camera;
    }
    void Scene::addSampler(hikari::Sampler *sampler)
    {
        this->sampler = sampler;
    }
    void Scene::add(Shape *object, Material *material)
    {
        auto primitive = new GeometricPrimitive(std::shared_ptr<Shape>(object), std::shared_ptr<Material>(material));
        primitives.push_back(std::shared_ptr<GeometricPrimitive>(primitive));
    }
    void Scene::addPrimitive(Primitive *primitive)
    {
        primitives.push_back(std::shared_ptr<Primitive>(primitive));
    }

    void Scene::addLight(Light *light)
    {
        //this->lights.push_back(std::make_shared<Light>(light));
    }
    void Scene::addMesh(Mesh *mesh, Material *material)
    {
        for (auto tri : mesh->tris)
        {
            // tri->m = & mesh->m_m;
            auto primitive = new GeometricPrimitive(std::shared_ptr<Triangle>(tri), std::shared_ptr<Material>(material));
            primitives.push_back(std::shared_ptr<GeometricPrimitive>(primitive));
        }

        std::cout << primitives.size();
    }

    void Scene::buildBVH()
    {
        printf(" - Generating BVH...\n\n");
        this->bvh = new BVHAccel(primitives, 4, BVHAccel::SplitMethod::SAH);
    }

    bool Scene::IntersectP(const Ray &ray) const
    {
        return this->bvh->IntersectP(bvh->root, ray);
    }
    bool Scene::Intersect(const Ray &ray, Intersection *isect) const
    {

        return this->bvh->Intersect(this->bvh->root, ray, isect);
    }
} // namespace hikari