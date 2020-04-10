#ifndef MATERIAL_H
#define MATERIAL_H


#include "hikari.h"
#include "Vectors.h"

namespace hikari {
    enum MaterialType {DIFF, SPEC, REFR, GLOSSY, TRANSMISSION};  // material types, used in radiance()
    enum class TransportMode { Radiance, Importance};
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

    class Material{
    private:
        //Texture tex;
    public:
        void ComputeScatteringFunctions(Intersection *intersection, bool allowMultipleLobes);
    };

//Vector3 getColorAt(double u, double v) const;
}



#endif //MATERIAL_H
