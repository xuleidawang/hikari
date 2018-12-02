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
	//number of samples per pixel
	int spp = argc==2?atoi(argv[1]):1;
  double spp_inv = 1./spp;
	//Ray cam(Vector3(50,52,295.6), Vector3(0,-0.042612,-1).normalize()); 
  // camera pos, target
  Camera camera(Vector3(50,52,295.6)+0*Vector3(0,-0.06,-1).normalize(), Vector3(50,52,295.6)+100*Vector3(0,-0.06,-1).normalize(),(int)600,(int)600);
  Scene scene = Scene();
  scene.add(dynamic_cast<Object*>(new Sphere (1e5, Vector3( 1e5+1,40.8,81.6), Vector3(),Vector3(.75,.25,.25),DIFF)));//Left
  scene.add(dynamic_cast<Object*>(new Sphere (1e5, Vector3(-1e5+99,40.8,81.6),Vector3(),Vector3(.25,.25,.75),DIFF)));//Right
  scene.add(dynamic_cast<Object*>(new Sphere (1e5, Vector3(50,40.8, 1e5),     Vector3(),Vector3(.75,.75,.75),DIFF)));//Back
  scene.add(dynamic_cast<Object*>(new Sphere (1e5, Vector3(50,40.8,-1e5+170), Vector3(),Vector3(),           DIFF)));//Front
  scene.add(dynamic_cast<Object*>(new Sphere (1e5, Vector3(50, 1e5, 81.6),    Vector3(),Vector3(.75,.75,.75),DIFF)));//Bottom
  scene.add(dynamic_cast<Object*>(new Sphere (1e5, Vector3(50,-1e5+81.6,81.6),Vector3(),Vector3(.75,.75,.75),DIFF)));//Top
  scene.add(dynamic_cast<Object*>(new Sphere (16.5,Vector3(27,16.5,47),       Vector3(),Vector3(1,1,1)*.999, SPEC)));//Mirror
  scene.add(dynamic_cast<Object*>(new Sphere (16.5,Vector3(73,16.5,78),       Vector3(),Vector3(1,1,1)*.999, REFR)));//Glass
  scene.add(dynamic_cast<Object*>(new Sphere (600, Vector3(50,681.6,81.6),Vector3(12,12,12),  Vector3(), DIFF)));//Light

  //scene.addMesh((new Mesh(Vector3(50,16.5,57), "../obj/cube.obj", Material(DIFF, Vector3(0, 0.85, 0)))) );

  //scene.add(dynamic_cast<Object*>(new Triangle(Vector3(27,45.5,47),Vector3(73,55.5,78),Vector3(50,60,80.6), new Material(DIFF,Vector3(0,.85,0)))));
  //scene.add(dynamic_cast<Object*>(new Triangle(Vector3(50,56.5,56.5),Vector3(51,57.5,56.5),Vector3(50,57.5,56.5), new Material(DIFF,Vector3(0,.85,0)))));

//  //scene.buildBVH();
//  Renderer renderer = Renderer(&scene, &camera);
//  renderer.render(spp);                       // Render image to pixel buffer
//  renderer.save_image();              // Save image

  // Print duration information
  time(&stop);
  double diff = difftime(stop, start);
  int hrs = (int)diff/3600;
  int mins = ((int)diff/60)-(hrs*60);
  int secs = (int)diff-(hrs*3600)-(mins*60);
  printf("\rRendering (%i samples): Complete!\nTime Taken: %i hrs, %i mins, %i secs\n\n", spp, hrs, mins, secs);

  Scene scene1=Scene();
  Camera camera1(Vector3(0,-5,2.5),Vector3(0,0,1),1280,720);
  scene1.add( dynamic_cast<Object*>(new Sphere(1000,Vector3(0,0,-1000), Vector3(), Vector3(1.0,1.0,1.0),DIFF)));
  scene1.add( dynamic_cast<Object*>(new Sphere(1000,Vector3(-1004,0,0), Vector3(), Vector3(0.85,0.4,0.4),DIFF)));
  scene1.add( dynamic_cast<Object*>(new Sphere(1000,Vector3(1004,0,0),  Vector3(), Vector3(0.4,0.4,0.85),DIFF)));
  scene1.add( dynamic_cast<Object*>(new Sphere(1000,Vector3(0,1006,0),  Vector3(), Vector3(),DIFF)));
  scene1.add( dynamic_cast<Object*>(new Sphere(100, Vector3(0,0,110),   Vector3(1,1,1)*.999,Vector3(1.0,1.0,1.0)*.999,DIFF)));
  //scene1.add( dynamic_cast<Object*>(new Mesh(Vector3(), "../obj/dragon2.obj", Material(DIFF, Vector3(0.9, 0.9, 0.9)))) );
  scene1.buildBVH();
  Renderer renderer1 =Renderer(&scene1,&camera1);
  renderer1.render(spp);
  renderer1.save_image();



  return 0;
}