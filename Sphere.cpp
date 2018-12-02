//
// Created by LEI XU on 11/28/18.
//
#include "Sphere.h"



bool Sphere::intersect(const Ray& ray){
    double b = 2 * (ray.direction.x * (ray.origin.x - pos.x)+ ray.direction.y * (ray.origin.y - pos.y)+ ray.direction.z * (ray.origin.z - pos.z));
    double c = (ray.origin.x - pos.x) * (ray.origin.x - pos.x) +(ray.origin.y - pos.y) * (ray.origin.y - pos.y) +(ray.origin.z - pos.z) * (ray.origin.z - pos.z) -radius * radius;
    double d = b * b - 4 * c;
    //Negative delta means no intersections
    if (d < 0.0) return false;
    double t = (-b - sqrt(d)) * 0.5;
    if (t > EPSILON) return true;
    t = (-b + sqrt(d)) * 0.5;
    if (t > EPSILON) return true;
    return false;
}

bool Sphere::intersect(const Ray& ray, Intersection *isect){
    double b = 2 * (ray.direction.x * (ray.origin.x - pos.x)+ ray.direction.y * (ray.origin.y - pos.y)+ ray.direction.z * (ray.origin.z - pos.z));
    double c = (ray.origin.x - pos.x) * (ray.origin.x - pos.x) +(ray.origin.y - pos.y) * (ray.origin.y - pos.y) +(ray.origin.z - pos.z) * (ray.origin.z - pos.z) -radius * radius;
    double d = b * b - 4 * c;
    //Negative delta means no intersections
    if (d < 0.0) return false;
    double t = (-b - sqrt(d)) * 0.5;
    if (t > EPSILON){
        if(t>ray.t_min&& t<ray.t_max){
            isect->coords = Vector3(ray.origin + ray.direction * t);
            isect->normal = Vector3(isect->coords - pos).normalize();
            isect->m = this->material;
            isect->obj = this;
            isect->distance = t;
        }

        return true;
    }
    t = (-b + sqrt(d)) * 0.5;
    if (t > EPSILON){
        if(t>ray.t_min&& t<ray.t_max){
            isect->coords = Vector3(ray.origin + ray.direction * t);
            isect->normal = Vector3(isect->coords - pos).normalize();
            isect->m = this->material;
            isect->obj = this;
            isect->distance = t;
        }
        return true;
    }

    return false;
}

Intersection Sphere::getIntersection(Ray _ray) {
    //Gets data about an intersection with a ray
    Intersection result;
    result.happened = false;

    double b = 2 * (_ray.direction.x * (_ray.origin.x - pos.x)
                    + _ray.direction.y * (_ray.origin.y - pos.y)
                    + _ray.direction.z * (_ray.origin.z - pos.z)
    );

    double c = (_ray.origin.x - pos.x) * (_ray.origin.x - pos.x) +
               (_ray.origin.y - pos.y) * (_ray.origin.y - pos.y) +
               (_ray.origin.z - pos.z) * (_ray.origin.z - pos.z) -
               radius * radius;

    double d = b * b - 4 * c;

    //Negative delta means no intersections
    if (d < 0) return result;

    double t = (-b - sqrt(d)) * 0.5;
    if (t > EPSILON) result.happened = true;
    else {
        t = (-b + sqrt(d)) * 0.5;
        if (t > EPSILON)
            result.happened = true;
        else
            return result;
    }

    result.coords = Vector3(_ray.origin + _ray.direction * t);
    result.normal = Vector3(result.coords - pos).normalize();
    result.m = this->material;
    result.obj = this;
    result.distance = t;

    return result;
}


Vector3 Sphere::getNormal(Vector3 position){
    Vector3 N = position-pos;
    return N.normalize();
}
Bounds3 Sphere::getBounds() {
    return Bounds3(Vector3(pos.x-radius, pos.y-radius, pos.z-radius),
                   Vector3(pos.x+radius, pos.y+radius, pos.z+radius));
}
