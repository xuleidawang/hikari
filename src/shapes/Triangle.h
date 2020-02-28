#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "src/core/Shape.h"
#include "Vectors.h"
#include <vector>
#include "Bounds3.h"
#include "src/core/Intersection.h"
#include "./lib/tiny_obj_loader/tiny_obj_loader.h"
class Triangle;
class Mesh : public Shape {

public:
	std::vector<tinyobj::shape_t> m_shapes;
    std::vector<tinyobj::material_t> m_materials;
    std::vector<Material> materials;

    Material m_m;	// Material
    //BVH bvh;


    std::vector<Triangle*> tris;
	Mesh(Vector3 p_, const char* file_path, Material m_);

	bool intersect(const Ray& ray, Intersection* intersection);
	Vector3 getNormal(Vector3 position);
	Bounds3 getBounds();

};

class Triangle: public Shape{
public:
	Vector3 v0, v1, v2; //vertices A, B ,C , counter-clockwise order 
	Vector3 e1, e2; //2 edges v1-v0, v2-v0;
	Vector3 t0,t1,t2;//texture coords
	Vector3 normal;
	Material *m;
// Triangle Public Methods

	Triangle(Vector3 _v0, Vector3 _v1, Vector3 _v2, Material *_m = NULL):v0(_v0),v1(_v1),v2(_v2),m(_m){
		e1 = v1-v0;
		e2 = v2-v0;
		normal = e1.cross(e2).normalize();
	}
	Triangle(Vector3 v0_, Vector3 v1_, Vector3 v2_, Vector3 t0_=Vector3(), Vector3 t1_=Vector3(), Vector3 t2_=Vector3(), Material *m_=NULL){
        v0=v0_, v1=v1_, v2=v2_, e1=v1-v0, e2=v2-v0, normal=e1.cross(e2).normalize();
        t0=t0_, t1=t1_, t2=t2_;
        m=m_;
    }
	bool intersect(const Ray& ray, Intersection* intersection);
	Vector3 getNormal(Vector3 position);
	Vector3 getBarycentric(Vector3 p);
	Bounds3 getBounds();
	Vector3 getMidpoint();
};


bool Triangle::intersect(const Ray& ray, Intersection* intersection){
	//Moller-Trumbore algorithm
	// P = (1-u-v)A + uB +vC 
	// P = wA + uB + vC
	// P = A + u(B-A) + v(C-A)
    if(ray.direction.dot(normal)>0)return false;
    double u,v ,t_tmp=0;
    Vector3 pvec = ray.direction.cross(e2);
    double det = e1.dot(pvec);
    if(fabs(det)<EPSILON)return false;

    double det_inv = 1./det;
    Vector3 tvec = ray.origin - v0;
    u = tvec.dot(pvec)*det_inv;
    if( u<0 || u>1)return false;
    Vector3 qvec = tvec.cross(e1);
    v = ray.direction.dot(qvec)*det_inv;
    if(v<0||u+v>1)return false;
    t_tmp = e2.dot(qvec)* det_inv;

    intersection->obj=this;
    intersection->m = this->m;
    intersection->happened=true;
    intersection->normal = this->normal;
    intersection->coords = t_tmp*ray.direction+ray.origin;
    intersection->distance =t_tmp;
    return intersection->happened;

}

Vector3 Triangle::getNormal(Vector3 position){return normal;}

// p = u*v0 + v*v1 + w*v2;
Vector3 Triangle::getBarycentric(Vector3 p){
		Vector3 v2_ = p - v0;
        double d00 = e1.dot(e1);
        double d01 = e1.dot(e2);
        double d11 = e2.dot(e2);
        double d20 = v2_.dot(e1);
        double d21 = v2_.dot(e2);
        double d = d00*d11 - d01*d01;
        double v = (d11*d20 - d01*d21) / d;
        double w = (d00*d21 - d01*d20) / d;
        double u = 1 - v - w;
        return Vector3(u, v, w);
}

Vector3 Triangle::getMidpoint(){
	return (v0+v1+v2)/3;
}

Mesh::Mesh(Vector3 p_, const char* file_path, Material m_) {

	m_p=p_, m_m=m_;

    std::string mtlbasepath;
    std::string inputfile = file_path;
    unsigned long pos = inputfile.find_last_of("/");
    mtlbasepath = inputfile.substr(0, pos+1);

    printf("Loading %s...\n", file_path);
    // Attempt to load mesh
	std::string err = tinyobj::LoadObj(m_shapes, m_materials, inputfile.c_str(), mtlbasepath.c_str());

	if (!err.empty()) {
		std::cerr << err << std::endl;
		exit(1);
	}
    long shapes_size, indices_size, materials_size;
    shapes_size = m_shapes.size();
    materials_size = m_materials.size();

    // Load materials/textures from obj
    // TODO: Only texture is loaded at the moment, need to implement material types and colours
    for (int i=0; i<materials_size; i++) {
        std::string texture_path = "";

        if (!m_materials[i].diffuse_texname.empty()){
            if (m_materials[i].diffuse_texname[0] == '/') texture_path = m_materials[i].diffuse_texname;
            texture_path = mtlbasepath + m_materials[i].diffuse_texname;
            materials.push_back( Material(DIFF, Vector3(1,1,1), Vector3()));
        }
        else {
            materials.push_back( Material(DIFF, Vector3(1,1,1), Vector3()) );
        }

    }
    // Load triangles from obj
    for (int i = 0; i < shapes_size; i++) {
        indices_size = m_shapes[i].mesh.indices.size() / 3;
        for (size_t f = 0; f < indices_size; f++) {

            // Triangle vertex coordinates
            Vector3 v0_ = Vector3(
                    m_shapes[i].mesh.positions[ m_shapes[i].mesh.indices[3*f] * 3     ],
                    m_shapes[i].mesh.positions[ m_shapes[i].mesh.indices[3*f] * 3 + 1 ],
                    m_shapes[i].mesh.positions[ m_shapes[i].mesh.indices[3*f] * 3 + 2 ]
            ) + m_p;

            Vector3 v1_ = Vector3(
                    m_shapes[i].mesh.positions[ m_shapes[i].mesh.indices[3*f + 1] * 3     ],
                    m_shapes[i].mesh.positions[ m_shapes[i].mesh.indices[3*f + 1] * 3 + 1 ],
                    m_shapes[i].mesh.positions[ m_shapes[i].mesh.indices[3*f + 1] * 3 + 2 ]
            ) + m_p;

            Vector3 v2_ = Vector3(
                    m_shapes[i].mesh.positions[ m_shapes[i].mesh.indices[3*f + 2] * 3     ],
                    m_shapes[i].mesh.positions[ m_shapes[i].mesh.indices[3*f + 2] * 3 + 1 ],
                    m_shapes[i].mesh.positions[ m_shapes[i].mesh.indices[3*f + 2] * 3 + 2 ]
            ) + m_p;

            Vector3 t0_, t1_, t2_;

            //Attempt to load triangle texture coordinates
            if (m_shapes[i].mesh.indices[3 * f + 2] * 2 + 1 < m_shapes[i].mesh.texcoords.size()) {
                t0_ = Vector3(
                        m_shapes[i].mesh.texcoords[m_shapes[i].mesh.indices[3 * f] * 2],
                        m_shapes[i].mesh.texcoords[m_shapes[i].mesh.indices[3 * f] * 2 + 1],
                        0
                );

                t1_ = Vector3(
                        m_shapes[i].mesh.texcoords[m_shapes[i].mesh.indices[3 * f + 1] * 2],
                        m_shapes[i].mesh.texcoords[m_shapes[i].mesh.indices[3 * f + 1] * 2 + 1],
                        0
                );

                t2_ = Vector3(
                        m_shapes[i].mesh.texcoords[m_shapes[i].mesh.indices[3 * f + 2] * 2],
                        m_shapes[i].mesh.texcoords[m_shapes[i].mesh.indices[3 * f + 2] * 2 + 1],
                        0
                );
            }
            else {
                t0_=Vector3();
                t1_=Vector3();
                t2_=Vector3();
            }
            if (m_shapes[i].mesh.material_ids[ f ] < materials.size())
                //tris.push_back(new Triangle(v0_, v1_, v2_, t0_, t1_, t2_, &materials[ m_shapes[i].mesh.material_ids[ f ] ]));
                tris.push_back(new Triangle(v0_, v1_, v2_, t0_, t1_, t2_, &m_m));
            else
                tris.push_back(new Triangle(v0_, v1_, v2_, t0_, t1_, t2_, &m_m));
        }
    }

    // Clean up
    m_shapes.clear();
    m_materials.clear();
    printf("\n");

}

// Check if ray intersects with mesh. Returns ObjectIntersection data structure
bool Mesh::intersect(const Ray& ray, Intersection* intersection) {
    int size = tris.size();

    //bool hit = node->hit(node, ray, t, tmin, normal, colour);
    //bool hit = bvh.getIntersection(ray, t, tmin, normal);
    //    std::cout<<"intersect with mesh"<<std::endl;
    for(int i =0;i<size;i++){
        Intersection* tmp = new Intersection();
        if(tris.at((unsigned)i)->intersect(ray, tmp)){
            intersection->happened=true;
            if(tmp->distance< intersection->distance){
                intersection = tmp;
            }
        }
    }
    //std::cout<<"Intersection with Mesh"<<std::endl;
    return intersection->happened;

}
Bounds3 Mesh::getBounds() {
    std::cout<<"This is a Mesh Bound, try something else"<<std::endl;
    Bounds3 box;

    return box;
}

Bounds3 Triangle::getBounds() {
    return Union(Bounds3(v0, v1), v2);
}


#endif// TRIANGLE_H