#ifndef RENDERER_H
#define RENDERER_H
#include <iostream>

#include "Vectors.h"
#include "Scene.h"
#include "Camera.h"
// Clamp double to min/max of 0/1
inline double clamp(double x){ return x<0 ? 0 : x>1 ? 1 : x; }
// Clamp to between 0-255
inline int toInt(double x){ return int(clamp(x)*255+.5); }

class Renderer {

private:
    Scene *m_scene;
    Camera *m_camera;
    Vector3 *m_pixel_buffer;

public:
    Renderer(Scene *scene, Camera *camera);
    void render(int samples=4);
    void save_image();

};

Renderer::Renderer(Scene *scene, Camera *camera) {
    m_scene = scene;
    m_camera = camera;
    m_pixel_buffer = new Vector3[m_camera->get_width()*m_camera->get_height()];
}

void Renderer::render(int samples) {
    int width = m_camera->get_width();
    int height = m_camera->get_height();
    double samples_recp = 1./samples;

    // Main Loop
    //#pragma omp parallel for schedule(dynamic, 1)       // OpenMP
    for (int y=0; y<height; y++){
        fprintf(stderr, "\rRendering (%i samples): %.2f%% ",samples, (double)y/height*100);                   // progress
        for (int x=0; x<width; x++){
            Vector3 color = Vector3();

            for (int a=0; a<samples; a++){
                Ray ray = m_camera->generate_ray(x, y, a>0);
                ray.origin = ray.origin + ray.direction*140;
                color = color + m_scene->castRay(ray,0);
            }
            int i = y*width+x;
            m_pixel_buffer[i] = color * samples_recp;
            m_pixel_buffer[i] = m_pixel_buffer[i] + Vector3(clamp(color.x),clamp(color.y),clamp(color.z))*.25; 
        }
    }
}

void Renderer::save_image(){
	int width = m_camera->get_width();
	int height = m_camera->get_height();

	FILE *f = fopen("./image.ppm", "w");         // Write image to PPM file.
   fprintf(f, "P3\n%d %d\n%d\n", width, height, 255); 
   for (int i=0; i < height*width; i++) 
   	fprintf(f,"%d %d %d ", toInt(m_pixel_buffer[i].x), toInt(m_pixel_buffer[i].y), toInt(m_pixel_buffer[i].z)); 
   std::cout<<std::endl;
}




#endif //RENDERER_H