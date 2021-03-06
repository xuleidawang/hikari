//
// Created by LEI XU on 2/29/20.
//
#include "Primitive.h"

namespace hikari {
    Primitive::~Primitive() {}
    const AreaLight *GeometricPrimitive::GetAreaLight() const{ return this->areaLight.get(); }
    const Material *GeometricPrimitive::GetMaterial() const{ return material.get();}
    
    GeometricPrimitive::GeometricPrimitive(const std::shared_ptr<Shape> &shape,
                                       const std::shared_ptr<Material> &material,
                                       const std::shared_ptr<AreaLight> &areaLight
                                        )
    : shape(shape),
    material(material),
    areaLight(areaLight){
    }

    GeometricPrimitive::GeometricPrimitive(const std::shared_ptr<Shape> &shape,
                                       const std::shared_ptr<Material> &material
                                        )
    : shape(shape),
    material(material){
    }

    bool GeometricPrimitive::Intersect(const Ray &r, Intersection *isect) const {
       float tHit;
       if (!shape->Intersect(r, isect)) return false;
       //r.t_max = tHit;
       isect->primitive = this;
       isect->happened = true;

       return isect->happened;
   }
   
   Bounds GeometricPrimitive::WorldBound() const { return shape->getBounds();}

   void GeometricPrimitive::ComputeScatteringFunctions( Intersection *isect, bool allowMultipleLobes) const {
       if (material)
       {
           material->ComputeScatteringFunctions(isect, allowMultipleLobes);
           //        CHECK_GE(Dot(isect->n, isect->normal), 0.);
       }
    }
}

