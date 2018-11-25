#ifndef SCENE_H
#define SCENE_H

#include "Ray.h"
#include "Vectors.h"
#include "Object.h"
#include "Intersection.h"
#include <vector>
class Object;
class Scene {
private:
    std::vector<Object*> m_objects;

public:
    Scene(){};
    void add(Object *object);
    Intersection intersect(const Ray &ray);
    Vector3 castRay(const Ray &ray, int depth);
};


void Scene::add(Object *object) {
    m_objects.push_back( object );
}
Intersection Scene::intersect(const Ray &ray){
  Intersection intersec,tmp;
  long size= m_objects.size();
  double t = std::numeric_limits<double>::max();

  for(int i =0;i<size;i++){
    tmp = m_objects.at((unsigned)i)->getIntersection(ray);

    if(!m_objects.at((unsigned)i)->intersect(ray))continue;
    else {
      tmp = m_objects.at((unsigned)i)->getIntersection(ray);
      if(tmp.distance<t){
        intersec = tmp;
        t = intersec.distance;
      }
    }
  }
  return intersec;
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
  Vector3 resColor(0.0,0.0,0.0),directColor(0.0,0.0,0.0), indirectColor(0.0,0.0,0.0);
  // find the intersected object 
  Intersection intersection = Scene::intersect(ray);
  if(!intersection.happened)return resColor;
  Material *m = &intersection.m;
  Vector3 x = intersection.coords;
  Vector3 n=intersection.normal;
  Vector3 nl=n.dot(ray.direction)<0?n:n*-1;// hitting at outside or inside of the intersected object
  Vector3 f=m->getColor();

  double p = fmax(fmax(f.x,f.y),f.z);// max refl 
  if (++depth>5){
    if (drand48()<p)f=f*(1/p);
    else return m->getEmission(); //R.R.
  }
  // Ideal DIFFUSE reflection 
  if(m->getType() == DIFF)
  {
    Vector3 d = getHemiRandomDirection(nl);
    return m->getEmission() + f*castRay(Ray(x,d),depth);
  }
 else if(m->getType()==SPEC){
   double roughness = 0.8;
   Vector3 reflected = ray.direction - n * 2 * n.dot(ray.direction);
   Vector3 refdirect (
     reflected.x + (drand48()-0.5)*roughness,
     reflected.y + (drand48()-0.5)*roughness,
     reflected.z + (drand48()-0.5)*roughness
     );

   Ray reflRay(x,refdirect.normalize());
   //Ray reflRay(x, reflected.normalize());
   //return object->e + f*(castRay(Ray(x,(ray.direction-n*2*n.dot(ray.direction)).normalize()),depth));
   return m->getEmission()+ f*castRay(reflRay, depth);
 }
 else if(m->getType() ==REFR){
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
   if(into)nnt = nc/nt;
   else nnt = nt/nc;
   double ddn=ray.direction.dot(nl);
   double cos2t = 1-nnt*nnt*(1-ddn*ddn);
   if(cos2t<0) return  m->getEmission() + f*(castRay(reflRay,depth)); // Total internal reflection
   Vector3 tdir = (ray.direction*nnt - n*((into?1:-1)*(ddn*nnt+sqrt(cos2t)))).normalize();
   double a=nt-nc, b=nt+nc, R0=a*a/(b*b), c = 1-(into?-ddn:tdir.dot(n));
   double Re=R0+(1-R0)*c*c*c*c*c,Tr=1-Re,P=.25+.5*Re,RP=Re/P,TP=Tr/(1-P);
   return m->getEmission() + f*(depth>2 ? (drand48()<P ?   // Russian roulette
     castRay(reflRay,depth)*RP:castRay(Ray(x,tdir),depth)*TP):
     castRay(reflRay,depth)*Re+castRay(Ray(x,tdir),depth)*Tr);
 }
  return Vector3();
}




#endif //SCENE_H