#include <iostream>
#include <limits>
#include "Vectors.h"
#include "Ray.h"
#include "Sphere.h"
#include "Intersection.h"
using namespace std;

#define IMAGE_WITDH 	600
#define IMAGE_HEIGHT 	600
#define MAX_DEPTH		5
typedef unsigned char u08;


// utilities
inline double clamp(double x){ return x<0 ? 0 : x>1 ? 1 : x; } 
inline int toInt(double x){ return int(pow(clamp(x),1/2.2)*255+.5); } 

//hardcode Cornell Box
Sphere spheres[] = {
//Scene: radius,    position,            emission,   color,           material 
   Sphere (1e5, Vector3( 1e5+1,40.8,81.6), Vector3(),Vector3(.75,.25,.25),DIFF),//Left 
   Sphere (1e5, Vector3(-1e5+99,40.8,81.6),Vector3(),Vector3(.25,.25,.75),DIFF),//Rght 
   Sphere (1e5, Vector3(50,40.8, 1e5),     Vector3(),Vector3(.75,.75,.75),DIFF),//Back 
   Sphere (1e5, Vector3(50,40.8,-1e5+170), Vector3(),Vector3(),           DIFF),//Frnt 
   Sphere (1e5, Vector3(50, 1e5, 81.6),    Vector3(),Vector3(.75,.75,.75),DIFF),//Botm 
   Sphere (1e5, Vector3(50,-1e5+81.6,81.6),Vector3(),Vector3(.75,.75,.75),DIFF),//Top 
   Sphere (16.5,Vector3(27,16.5,47),       Vector3(),Vector3(1,1,1)*.999, SPEC),//Mirr 
   Sphere (16.5,Vector3(73,16.5,78),       Vector3(),Vector3(1,1,1)*.999, REFR),//Glas 
   Sphere (600, Vector3(50,681.6-.27,81.6),Vector3(12,12,12),  Vector3(), DIFF) //Light 
 }; 

Intersection getCloseIntersec(Ray ray){
  Intersection intersec;
  double t = std::numeric_limits<double>::max();  
  for(int id= 0;id<9;id++){
    if(!spheres[id].intersect(ray))continue;
    else {
      Intersection inter = spheres[id].getIntersection(ray);
      if(inter.distance<t){
        intersec = inter;
        t = intersec.distance;
      }
    }
  }
  return intersec;
}


Vector3  castRay(const Ray ray, int depth){
  Vector3 resColor(0.0,0.0,0.0),directColor(0.0,0.0,0.0), indirectColor(0.0,0.0,0.0);

  // find the intersected object 
  Intersection intersection = getCloseIntersec(ray);
  if(!intersection.happened)return resColor;
  Sphere* object = (Sphere*)intersection.obj;
  
  //return object->color;
  Vector3 x = intersection.coords;
  Vector3 n=intersection.normal;
  Vector3 nl=n.dot(ray.direction)<0?n:n*-1;
  Vector3 f=object->color; 

  double p = max(max(f.x,f.y),f.z);// max refl 
  if (++depth>5){
    if (drand48()<p)f=f*(1/p);
    else return object->e; //R.R. 
  }
  // Ideal DIFFUSE reflection 
  if(object->refl == DIFF)
  {

    double r1=2*M_PI*drand48(), r2=drand48(), r2s=sqrt(r2); 
    Vector3 w=nl, u=((fabs(w.x)>.1?Vector3(0,1):Vector3(1)).cross(w)).normalize(), v=w.cross(u); 
    Vector3 d = (u*cos(r1)*r2s + v*sin(r1)*r2s + w*sqrt(1-r2)).normalize(); 
    // directColor = 1* clamp(n.dot(-ray.direction));
    // indirectColor = r1*castRay(Ray(x,d),depth);
    return object->e + f*castRay(Ray(x,d),depth);
  }
  else if(object->refl==SPEC){
    double roughness = 0.8;
        Vector3 reflected = ray.direction - n * 2 * n.dot(ray.direction);
        Vector3 refdirect (
            reflected.x + (drand48()-0.5)*roughness,
            reflected.y + (drand48()-0.5)*roughness,
            reflected.z + (drand48()-0.5)*roughness
        );
    return f*castRay(Ray(x, refdirect.normalize()), depth);
  }
  return resColor;
}


int main(int argc, char** argv){	
	//number of samples per pixel
	int spp = argc==2?atoi(argv[1])/4:1  ;
	Ray cam(Vector3(50,52,295.6), Vector3(0,-0.042612,-1).normalize()); // camera pos, dir 
	Vector3 cx(IMAGE_WITDH*.5135/IMAGE_HEIGHT), cy=(cx.cross(cam.direction)).normalize()*.5135, r;
  Vector3 *c=new Vector3[IMAGE_HEIGHT*IMAGE_WITDH]; 
  for (int y=0; y<IMAGE_HEIGHT; y++){                       // Loop over image rows 
    fprintf(stderr,"\rRendering (%d spp) %5.2f%%",spp*4, 100.*y/(IMAGE_HEIGHT-1)); 
    for (unsigned short x=0, Xi[3]={0,0,y*y*y}; x<IMAGE_WITDH; x++)   // Loop cols 
			for (int sy=0, i=(IMAGE_HEIGHT-y-1)*IMAGE_WITDH+x; sy<2; sy++)     // 2x2 subpixel rows 
				for (int sx=0; sx<2; sx++, r=Vector3()){        // 2x2 subpixel cols 
           			for (int s=0; s<spp; s++){ 
           				double r1=2*erand48(Xi), dx=r1<1 ? sqrt(r1)-1: 1-sqrt(2-r1); 
           				double r2=2*erand48(Xi), dy=r2<1 ? sqrt(r2)-1: 1-sqrt(2-r2); 
           				Vector3 d = cx*( ( (sx+.5 + dx)/2 + x)/IMAGE_WITDH - .5) + 
           				cy*( ( (sy+.5 + dy)/2 + y)/IMAGE_HEIGHT - .5) + cam.direction; 
           				r = r + castRay(Ray(cam.origin+d*140,d.normalize()),0)*(1./spp); 
           				} // Camera rays are pushed ^^^^^ forward to start in interior 
        c[i] = c[i] + Vector3(clamp(r.x),clamp(r.y),clamp(r.z))*.25; 
         } 
   } 
   FILE *f = fopen("./results/image.ppm", "w");         // Write image to PPM file. 
   fprintf(f, "P3\n%d %d\n%d\n", IMAGE_WITDH, IMAGE_HEIGHT, 255); 
   for (int i=0; i<IMAGE_HEIGHT*IMAGE_WITDH; i++) 
     fprintf(f,"%d %d %d ", toInt(c[i].x), toInt(c[i].y), toInt(c[i].z)); 
   cout<<endl;
}