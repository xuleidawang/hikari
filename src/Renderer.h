#pragma once
#include <iostream>

#include "Vectors.h"
#include "src/core/Scene.h"
#include "src/core/Camera.h"

namespace hikari {
    // Clamp double to min/max of 0/1
    inline double clamp(double x){ return x<0 ? 0 : x>1 ? 1 : x; }
    // Clamp to between 0-255
    inline int toInt(double x){ return int(clamp(x)*255+.5); }

    class Renderer {

    private:
        const Camera *camera;
        const Scene *scene;
        Vector3 *m_pixel_buffer;

    public:
        Renderer(Scene *scene);
        void render(int samples=4,double spp_inv = 0.25);
        void save_image();

    };

    Renderer::Renderer(Scene *scene) {
        this->scene = scene;
        this->camera = scene->getCamera();
        m_pixel_buffer = new Vector3[camera->get_width()*camera->get_height()];
    }

    void Renderer::render(int samples,double spp_recp) {
        printf("Starting main rendering loop...");
        int width = camera->get_width();
        int height = camera->get_height();
        Sampler *sampler = scene->getSampler();
        Integrator *integrator = scene->getIntegrator();
        
        // Main Loop
        #pragma omp parallel for schedule(dynamic, 1)       // OpenMP
        for (int y=0; y<height; y++)
        {
            fprintf(stderr, "\rRendering (%i samples): %.2f%% ",samples, (double)y/height*100);                   // progress
            for (int x=0; x<width; x++)
            {
                Vector3 color = Vector3(0.0);
                for (int a=0; a<samples; a++)
                {
                    Ray ray = camera->generate_ray(x, y, a>0);
                    // color = color + m_scene->castRay(ray,0);
                    //Li(const Ray &ray, const Scene &scene, Sampler &sampler, int depth = 0)
                    color = color + integrator->Li(ray, *scene, *sampler, 0);
                }
                
                int i = y*width+x;
                m_pixel_buffer[i] = color * spp_recp;
                m_pixel_buffer[i] = m_pixel_buffer[i] + Vector3(clamp(color.x),clamp(color.y),clamp(color.z))*.25;
            }
        }
    }



    void Renderer::save_image(){
        int width = camera->get_width();
        int height = camera->get_height();

        FILE *f = fopen("./image.ppm", "w");         // Write image to PPM file.
        fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
        for (int i=0; i < height*width; i++)
            fprintf(f,"%d %d %d ", toInt(m_pixel_buffer[i].x), toInt(m_pixel_buffer[i].y), toInt(m_pixel_buffer[i].z));
        std::cout<<std::endl;
    }
}


