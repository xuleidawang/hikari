//
// Created by LEI XU on 3/6/20.
//
#include "parser.h"
#include "Scene.h"
#include "Camera.h"
#include "time.h"

using namespace std;
using namespace hikari;

void render(Scene *scene, Sampler *sampler)
{
    const Camera *camera = scene->getCamera();
    Integrator *integrator = scene->getIntegrator();
}


int main(int argc, char** argv){
    time_t start, stop;
    time(&start);               // Start execution timer
    // number of samples per pixel
    int spp = argc==2?atoi(argv[1]):1;
    double spp_recp = 1/spp;

    Scene *cornellBox = new Scene();
    // camera pos, target
    loadScene(cornellBox);
    Camera camera1(Vector3(0,-20,5),Vector3(0,0,1),1280,720);

    cornellBox->buildBVH();

    // Renderer renderer1 =Renderer(&cornellBox,&camera1);
    // renderer1.render(spp, spp_recp);
    // renderer1.save_image();

    time(&stop);
    double diff = difftime(stop, start);
    int hrs = (int)diff/3600;
    int mins = ((int)diff/60)-(hrs*60);
    int secs = (int)diff-(hrs*3600)-(mins*60);
    printf("\rRendering (%i samples): Complete!\nTime Taken: %i hrs, %i mins, %i secs\n\n", spp, hrs, mins, secs);

    return 0;
}
