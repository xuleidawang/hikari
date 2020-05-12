//
// Created by LEI XU on 3/6/20.
//

#include "Camera.h"
#include "Scene.h"
#include "sampling.h"
#include "Integrator.h"
#include "Renderer.h"
#include "time.h"
#include "shapes/Sphere.h"
#include "shapes/Triangle.h"
#include "materials/matte.h"
#include "integrators/whitted.h"
#include "sampler.h"

using namespace std;
using namespace hikari;


void render(Scene *scene, Sampler *sampler)
{
    const Camera *camera = scene->getCamera();
    const Integrator *integrator = scene->getIntegrator();

}


int main(int argc, char** argv){
    time_t start, stop;
    time(&start);               // Start execution timer
    // number of samples per pixel
    int spp = argc==2?atoi(argv[1]):1;
    double spp_recp = 1/spp;

    Scene *cornellBox = new Scene();
    // camera pos, target
    Camera camera1(Vector3(0,-20,5),Vector3(0,0,1),1280,720);


    MatteMaterial* red = new MatteMaterial( Vector3(1.0, 0.0,0.0) );
    cornellBox->add( dynamic_cast<Shape*>(new Sphere(1000,Vector3(0,0,-1000), Vector3(), Vector3(1.0,1.0,1.0))), red);
    cornellBox->add( dynamic_cast<Shape*>(new Sphere(1000,Vector3(-1004,0,0), Vector3(), Vector3(0.85,0.4,0.4))), red);
    cornellBox->add( dynamic_cast<Shape*>(new Sphere(1000,Vector3(1004,0,0),  Vector3(), Vector3(0.4,0.4,0.85))), red);
    cornellBox->add( dynamic_cast<Shape*>(new Sphere(1000,Vector3(0,1006,0),  Vector3(), Vector3(1.0,1.0,1.0))), red);
    cornellBox->add( dynamic_cast<Shape*>(new Sphere(1000,Vector3(0,1006,0),  Vector3(), Vector3(1.0,1.0,1.0))), red);
    // CornellBox->add( dynamic_cast<Shape*>(new Sphere(100, Vector3(0,0,110),   Vector3(1,1,1)*.999,Vector3(1.0,1.0,1.0)*.999,DIFF))); //light
    const char* path = "../scene/dragon2.scene";
    Mesh dragon(Vector3(0.0,0.0,0.0), "../scene/dragon2.obj");
    cornellBox->addMesh(&dragon, new MatteMaterial(Vector3(0.99, 0.84, 0)));
    cornellBox->buildBVH();
    
    cornellBox->addCamera(&camera1);

    //                 spp, sampler dimension
    PixelSampler *sampler = new PixelSampler(4, 2);
    WhittedIntegrator *whitted = new WhittedIntegrator(4, make_shared<Camera>(camera1), make_shared<Sampler>(sampler));
    
    cornellBox->addIntegrator(whitted);
    Renderer renderer1 = Renderer(cornellBox);
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
