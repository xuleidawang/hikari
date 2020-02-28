#ifndef MATERIAL_H
#define MATERIAL_H


#include "Vectors.h"

enum MaterialType {DIFF, SPEC, REFR, GLOSSY, TRANSMISSION};  // material types, used in radiance()

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
	inline Material(MaterialType t=DIFF, Vector3 c=Vector3(1,1,1), Vector3 e=Vector3(0,0,0));
	inline MaterialType getType();
	inline Vector3 getColor();
	inline Vector3 getColorAt(double u, double v);
	inline Vector3 getEmission();

	
};

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
//Vector3 getColorAt(double u, double v) const;


#endif //MATERIAL_H
