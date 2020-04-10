//
// Created by LEI XU on 4/9/20.
//

#include "Material.h"

namespace hikari {

    Material::Material(MaterialType t, Vector3 c, Vector3 e){
        m_type = t;
        m_color = c;
        m_emission = e;
    }

    MaterialType Material::getType(){return m_type;}
    Vector3 Material::getColor(){return m_color;}
    Vector3 Material::getEmission() {return m_emission;}
    Vector3 Material::getColorAt(double u, double v) {
        return Vector3();
    }

    void Material::ComputeScatteringFunctions(hikari::Intersection *intersection, bool allowMultipleLobes)
    {
    }
}