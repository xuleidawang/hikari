#ifndef CAMERA_H
#define CAMERA_H

#include "Vectors.h"
#include "Ray.h"

class Camera{
private:
	Vector3 pos;
	Vector3 lootAt;
	Vector3 up;
	double ratio;
	int width;
	int height;

public:
	Camera(Vector3 _pos, Vector3 _lookAt, int width, int height);
	int get_width();
	int get_height();
	Ray generate_ray(int x,int y);
	
}