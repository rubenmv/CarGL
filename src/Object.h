#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include <map>

#include "Clock.h"

class Material;

class Object {
public:
	// Tipos de datos para el modelo
	typedef struct
	{
		std::string name;

		float ambient[3];
		float diffuse[3];
		float specular[3];
		float transmittance[3];
		float emission[3];
		float shininess;
		float ior; // indice de refraccion
		int shader;

		std::string ambient_texname;
		std::string diffuse_texname;
		std::string specular_texname;
		std::string normal_texname;
		std::map<std::string, std::string> unknown_parameter;
	} material_t;

	typedef struct
	{
		std::vector<float>          positions;
		std::vector<float>          normals;
		std::vector<float>          texcoords;
		std::vector<unsigned int>   indices;
	} mesh_t;

	typedef struct
	{
		std::string  name;
		material_t   material;
		mesh_t       mesh;
	} shape_t;

    Object(const char* fileName, float position[3]);
    virtual ~Object();

    std::vector<shape_t> model;

    float position[3];
    float rotation[3];
    // Establece una rotacion constante en los ejes indicados (valores 0,1)
    void setConstantRotation(short int x, short int y, short int z, float rotation = 0.0);

    void draw();

private:
    Material* material; // Este contiene la textura
    // ID de la display list
    int id;
    // Genera la display list para este objecto
    void createDisplayList();

    bool constantRotation;

    Clock timer;
};

#endif // OBJECT_H
