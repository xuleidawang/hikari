#include <iostream>
#include <limits>
#include "Vectors.h"
#include "Ray.h"
#include "Object.h"
#include "Sphere.h"
#include "Intersection.h"
#include "Material.h"
#include "Camera.h"
#include "Triangle.h"
#include "Scene.h"
#include "Renderer.h"
#include "time.h"
using namespace std;

#define IMAGE_WIDTH 	600
#define IMAGE_HEIGHT 	600
#define MAX_DEPTH		5
typedef unsigned char u08;


// utilities

inline double Lerp(double t, double x, double y){return (1-t)*x+t*y;}
////hard code Cornell Box
//Sphere spheres[] = {
////Scene: radius,    position,            emission,   color,           material
//   Sphere (1e5, Vector3( 1e5+1,40.8,81.6), Vector3(),Vector3(.75,.25,.25),DIFF),//Left
//   Sphere (1e5, Vector3(-1e5+99,40.8,81.6),Vector3(),Vector3(.25,.25,.75),DIFF),//Rght
//   Sphere (1e5, Vector3(50,40.8, 1e5),     Vector3(),Vector3(.75,.75,.75),DIFF),//Back
//   Sphere (1e5, Vector3(50,40.8,-1e5+170), Vector3(),Vector3(),           DIFF),//Frnt
//   Sphere (1e5, Vector3(50, 1e5, 81.6),    Vector3(),Vector3(.75,.75,.75),DIFF),//Botm
//   Sphere (1e5, Vector3(50,-1e5+81.6,81.6),Vector3(),Vector3(.75,.75,.75),DIFF),//Top
//   Sphere (16.5,Vector3(27,16.5,47),       Vector3(),Vector3(1,1,1)*.999, SPEC),//Mirr
//   //Sphere (16.5,Vector3(50,30.5,50),       Vector3(),Vector3(1,1,1)*.999, SPEC),//Mirr
//   Sphere (16.5,Vector3(73,16.5,78),       Vector3(),Vector3(1,1,1)*.999, REFR),//Glas
//   Sphere (600, Vector3(50,681.6-.27,81.6),Vector3(12,12,12),  Vector3(), DIFF) //Light
//
// };




int main(int argc, char** argv){
  time_t start, stop;
  time(&start);               // Start execution timer
  // number of samples per pixel
  int spp = argc==2?atoi(argv[1]):1;
  double spp_inv = 1./spp;
  // camera pos, target

  // Print duration information


  Scene scene1=Scene();
  Camera camera1(Vector3(0,-20,5),Vector3(0,0,1),1280,720);
  scene1.add( dynamic_cast<Object*>(new Sphere(1000,Vector3(0,0,-1000), Vector3(), Vector3(1.0,1.0,1.0),DIFF)));
  scene1.add( dynamic_cast<Object*>(new Sphere(1000,Vector3(-1004,0,0), Vector3(), Vector3(0.85,0.4,0.4),DIFF)));
  scene1.add( dynamic_cast<Object*>(new Sphere(1000,Vector3(1004,0,0),  Vector3(), Vector3(0.4,0.4,0.85),DIFF)));
  scene1.add( dynamic_cast<Object*>(new Sphere(1000,Vector3(0,1006,0),  Vector3(), Vector3(1.0,1.0,1.0),DIFF)));
  scene1.add( dynamic_cast<Object*>(new Sphere(1000,Vector3(0,1006,0),  Vector3(), Vector3(1.0,1.0,1.0),DIFF)));
  scene1.add( dynamic_cast<Object*>(new Sphere(100, Vector3(0,0,110),   Vector3(1,1,1)*.999,Vector3(1.0,1.0,1.0)*.999,DIFF))); //light

  scene1.add( dynamic_cast<Object*>(new Sphere(1.5,Vector3(-1.5,0,1.5),  Vector3(), Vector3(1.0,1.0,1.0),SPEC)));
  scene1.add( dynamic_cast<Object*>(new Sphere(1.5,Vector3(2,-2,1.5),  Vector3(), Vector3(1.0,1.0,1.0),REFR)));
  scene1.addMesh((new Mesh(Vector3(0,0,2.5), "../obj/dragon2.obj", Material(DIFF, Vector3(0.99, 0.84, 0)))) );

  scene1.buildBVH();
  Renderer renderer1 =Renderer(&scene1,&camera1);
  renderer1.render(spp);
  renderer1.save_image();

  time(&stop);
  double diff = difftime(stop, start);
  int hrs = (int)diff/3600;
  int mins = ((int)diff/60)-(hrs*60);
  int secs = (int)diff-(hrs*3600)-(mins*60);
  printf("\rRendering (%i samples): Complete!\nTime Taken: %i hrs, %i mins, %i secs\n\n", spp, hrs, mins, secs);

  return 0;
}