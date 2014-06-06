/*
Enumerados y tipos globales
*/

#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <map>

struct Vector3
{
	float x;
	float y;
	float z;
	// Estilo c++ para inicializar mediante constructor
	Vector3(float x = 0, float y = 0, float z = 0) { this->x = x; this->y = y; this->z = z;}
};



enum ID
{
	CARRETERA = 10,
	ACERA,
	ROTONDA,
	COCHE,
	FAROLA,
	EDIFICIO,
	CUBO_BASURA,
	PAPELERA,
	SENAL,
	BANCO,
	SELECCION
};

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

#endif // GLOBAL_H
