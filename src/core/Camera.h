#pragma once

#include "Ray.h"

namespace hikari {
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
        int get_width()const;
        int get_height()const;
        Ray generate_ray(int x,int y, bool jitter) const;

    };
}
