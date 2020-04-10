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
        MaterialType m_type;
        Vector3 m_color;
        Vector3 m_emission;
        //Texture tex;
    public:
        //Material(){m_type=DIFF;m_color=Vector3(1.0,1.0,1.0);m_emission=Vector3(0.0,0.0,0.0);}
        inline Material(MaterialType t=DIFF, Vector3 c= Vector3(1,1,1), Vector3 e=Vector3(0,0,0));
        inline MaterialType getType();
        inline Vector3 getColor();
        inline Vector3 getColorAt(double u, double v);
        inline Vector3 getEmission();
        void ComputeScatteringFunctions(Intersection *intersection, bool allowMultipleLobes);
    };

//Vector3 getColorAt(double u, double v) const;
}



#endif //MATERIAL_H
