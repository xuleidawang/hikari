//
// Created by LEI XU on 3/6/20.
//
#include <string>
#include "Camera.h"
#include "lib/minipbrt.h"
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

int main(int argc, char **argv)
{
    cout << "---------- Starting Hikari Renderer ----------\n";

    time_t start, stop;
    time(&start); // Start execution timer

    string fileName;
    // number of samples per pixel
    const int spp = 4;
    double spp_recp = 1 / spp;

    //TODO parse scene with minipbrt
    minipbrt::Loader loader;
    if (loader.load(argv[1]))
    {
        minipbrt::Scene *scene = loader.take_scene();
        cout << "PBRT scene load succeeded.\n";
        delete scene;
    }
    else
    {
        const minipbrt::Error *err = loader.error();
        fprintf(stderr, "[%s, line %lld, column %lld] %s\n",
                err->filename(), err->line(), err->column(), err->message());
    }
    Scene *scene = new Scene();
    // // camera pos, target
    // Camera camera1(Vector3(0,-20,5),Vector3(0,0,1),100,100);

    scene->buildBVH();

    //spp, sampler dimension
    PixelSampler *sampler = new PixelSampler(4, 2);
    //WhittedIntegrator *whitted = new WhittedIntegrator(4, make_shared<Camera>(camera1), make_shared<PixelSampler>(*sampler));
    //scene->addIntegrator(whitted);
    scene->addSampler(sampler);

    //TODO pass in the camera.
    Renderer renderer1 = Renderer(scene);
    renderer1.render(spp, spp_recp);
    renderer1.save_image();

    time(&stop);
    double diff = difftime(stop, start);
    int hrs = (int)diff / 3600;
    int mins = ((int)diff / 60) - (hrs * 60);
    int secs = (int)diff - (hrs * 3600) - (mins * 60);
    printf("\rRendering (%i samples): Complete!\nTime Taken: %i hrs, %i mins, %i secs\n\n", spp, hrs, mins, secs);

    return 0;
}
