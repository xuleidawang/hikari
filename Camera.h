#ifndef CAMERA_H
#define CAMERA_H

#include "Vectors.h"
#include "Ray.h"

class Camera{
private:
	Vector3 pos,direction;
	Vector3 target;
	Vector3 m_x_direction, m_y_direction;
	Vector3 up;
	double ratio;
	int width;
	int height;
	double m_width_recp, m_height_recp;
	double m_x_spacing;
    double m_x_spacing_half;
    double m_y_spacing;
    double m_y_spacing_half;

public:
	Camera(Vector3 _pos, Vector3 _target, int width, int height);
	int get_width();
	int get_height();
	Ray generate_ray(int x,int y, bool jitter);

};
Ray Camera::generate_ray(int x,int y, bool jitter){
	double x_jitter;
    double y_jitter;


    // If jitter == true, jitter point for anti-aliasing
    if (jitter) {
        x_jitter = (drand48() * m_x_spacing) - m_x_spacing_half;
        y_jitter = (drand48() * m_y_spacing) - m_y_spacing_half;

    }
    else {
        x_jitter = 0;
        y_jitter = 0;
    }

    Vector3 pixel = pos + direction*2;
    pixel = pixel - m_x_direction*ratio + m_x_direction*((x * 2 * ratio)*m_width_recp) + x_jitter;
    pixel = pixel + m_y_direction - m_y_direction*((y * 2.0)*m_height_recp + y_jitter);

    return Ray(pos, (pixel-pos).normalize());

}
Camera::Camera(Vector3 _pos, Vector3 _target, int _width, int _height):
pos(_pos),target(_target),width(_width),height(_height){
	m_width_recp = 1./width;
	m_height_recp = 1./height;
	ratio = (double)width/height;
    direction = (target - pos).normalize();
    m_x_direction = Vector3(0, 0, 1).cross(direction * -1).normalize();
    m_y_direction = m_x_direction.cross(direction).normalize();

    m_x_spacing = (2.0 * ratio)/(double)width;
    m_y_spacing = (double)2.0/(double)height;
    m_x_spacing_half = m_x_spacing * 0.5;
    m_y_spacing_half = m_y_spacing * 0.5;
};

int Camera::get_width(){return width;}
int Camera::get_height(){return height;}


#endif//CAMERA_H