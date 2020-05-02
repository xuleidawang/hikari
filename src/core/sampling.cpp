#include "sampling.h"

namespace hikari {
    Vector2 ConcentricSampleDisk(const Vector2 &u)
    {
        //Map uniform random number to [-1, 1]^2 
        Vector2 uOffset = 2.f * u - Vector2(1,1);

        //Handle degeneracy at the origin
        if(uOffset.x ==0 && uOffset.y ==0)
        return Vector2(0,0);

        //Apply concentric mapping to point
        float theta, r;
        if(std::abs(uOffset.x) > std::abs(uOffset.y)){
            r = uOffset.x;
            theta = (PI/4) * (uOffset.y / uOffset.x);
        }
        else{
            r = uOffset.y;
            theta = (PI/2) - (PI/4) * (uOffset.x / uOffset.y); 
        }
        return r * Vector2(std::cos(theta), std::sin(theta));
    }
}