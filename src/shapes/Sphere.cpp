//
// Created by LEI XU on 11/28/18.
//
#include "Sphere.h"

namespace hikari {
    bool Sphere::intersect(const Ray& ray, Intersection *intersection) {
        double b = 2 * (ray.direction.x * (ray.origin.x - pos.x)+ ray.direction.y * (ray.origin.y - pos.y)+ ray.direction.z * (ray.origin.z - pos.z));
        double c = (ray.origin.x - pos.x) * (ray.origin.x - pos.x) +(ray.origin.y - pos.y) * (ray.origin.y - pos.y) +(ray.origin.z - pos.z) * (ray.origin.z - pos.z) -radius * radius;
        double d = b * b - 4 * c;
        //Negative delta means no intersections
        if (d < 0.0) return false;
        double t = (-b - sqrt(d)) * 0.5;
        if (t > EPSILON){
            if(t>ray.t_min&& t<ray.t_max){
                intersection->happened = true;
                intersection->coords = Vector3(ray.origin + ray.direction * t);
                intersection->normal = Vector3(intersection->coords - pos).normalize();
                intersection->m = &this->material;
                intersection->obj = this;
                intersection->distance = t;
            }

            return true;
        }
        t = (-b + sqrt(d)) * 0.5;
        if (t > EPSILON){
            if(t>ray.t_min&& t<ray.t_max){
                intersection->happened = true;
                intersection->coords = Vector3(ray.origin + ray.direction * t);
                intersection->normal = Vector3(intersection->coords - pos).normalize();
                intersection->m = &this->material;
                intersection->obj = this;
                intersection->distance = t;
            }
            return true;
        }

        return false;
    }

    Vector3 Sphere::getNormal(Vector3 position){
        Vector3 N = position-pos;
        return N.normalize();
    }
    Bounds Sphere::getBounds() const {
        return Bounds(Vector3(pos.x-radius, pos.y-radius, pos.z-radius),
                       Vector3(pos.x+radius, pos.y+radius, pos.z+radius));
    }
}

