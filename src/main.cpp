#include <iostream>
#include <limits>
#include "Vectors.h"
#include "src/core/Ray.h"
#include "src/core/Shape.h"
#include "src/shapes/Sphere.h"
#include "src/core/Intersection.h"
#include "src/core/Material.h"
#include "src/core/Camera.h"
#include "src/shapes/Triangle.h"
#include "src/core/Scene.h"
#include "Renderer.h"
#include "time.h"
using namespace std;

#define IMAGE_WIDTH 	500
#define IMAGE_HEIGHT 	500
#define MAX_DEPTH		5
typedef unsigned char u08;


// utilities

inline double Lerp(double t, double x, double y){return (1-t)*x+t*y;}

int main(int argc, char** argv){
    time_t start, stop;
    time(&start);               // Start execution timer
    // number of samples per pixel
    int spp = argc==2?atoi(argv[1]):1;
    double spp_recp = 1/spp;
    
    Scene cornellBox=Scene();
    // camera pos, target
    Camera camera1(Vector3(0,-20,5),Vector3(0,0,1),1280,720);
    cornellBox.add( dynamic_cast<Shape*>(new Sphere(1000,Vector3(0,0,-1000), Vector3(), Vector3(1.0,1.0,1.0),DIFF)));
    cornellBox.add( dynamic_cast<Shape*>(new Sphere(1000,Vector3(-1004,0,0), Vector3(), Vector3(0.85,0.4,0.4),DIFF)));
    cornellBox.add( dynamic_cast<Shape*>(new Sphere(1000,Vector3(1004,0,0),  Vector3(), Vector3(0.4,0.4,0.85),DIFF)));
    cornellBox.add( dynamic_cast<Shape*>(new Sphere(1000,Vector3(0,1006,0),  Vector3(), Vector3(1.0,1.0,1.0),DIFF)));
    cornellBox.add( dynamic_cast<Shape*>(new Sphere(1000,Vector3(0,1006,0),  Vector3(), Vector3(1.0,1.0,1.0),DIFF)));
    cornellBox.add( dynamic_cast<Shape*>(new Sphere(100, Vector3(0,0,110),   Vector3(1,1,1)*.999,Vector3(1.0,1.0,1.0)*.999,DIFF))); //light

    cornellBox.add( dynamic_cast<Shape*>(new Sphere(1.5,Vector3(-1.5,0,1.5),  Vector3(), Vector3(1.0,1.0,1.0),SPEC)));
    cornellBox.add( dynamic_cast<Shape*>(new Sphere(1.5,Vector3(2,-2,1.5),  Vector3(), Vector3(1.0,1.0,1.0),REFR)));
    cornellBox.addMesh((new Mesh(Vector3(0,0,2.5), "../obj/dragon2.obj", Material(DIFF, Vector3(0.99, 0.84, 0)))) );

    cornellBox.buildBVH();
    Renderer renderer1 =Renderer(&cornellBox,&camera1);
    renderer1.render(spp, spp_recp);
    renderer1.save_image();

    time(&stop);
    double diff = difftime(stop, start);
    int hrs = (int)diff/3600;
    int mins = ((int)diff/60)-(hrs*60);
    int secs = (int)diff-(hrs*3600)-(mins*60);
    printf("\rRendering (%i samples): Complete!\nTime Taken: %i hrs, %i mins, %i secs\n\n", spp, hrs, mins, secs);

    return 0;
}