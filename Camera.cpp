#include "Camera.h"
#include "Ray.h"

Camera::Camera(Vector3 _pos, Vector3 _lookAt, int _width, int _height):
pos(_pos),lookAt(_lookAt),width(_width),height(_height){};

int get_width(){return width};
int get_height(){return height};

Ray generate_ray(int x,int y){
	return Ray();
}