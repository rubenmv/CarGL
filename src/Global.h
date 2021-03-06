/*
Enumerados y tipos globales
*/

#ifndef GLOBAL_H
#define GLOBAL_H

#define PI  3.14159265358979

#include <string>
#include <map>

#include <gl/gl.h>

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
	RUEDA_D, // Rueda delantera
	RUEDA_T, // Rueda trasera (no rotan)
	VOLANTE,
	LUNA_COCHE,
	FAROLA,
	EDIFICIO,
	CUBO_BASURA,
	PAPELERA,
	SENAL,
	BANCO,
	SELECCION
};

typedef struct
{
	const char* name;
	bool tracing;
	Vector3 position; // Posicion relativa en el caso de seguimiento (tracing)
	Vector3 rotation;
} Camera;

typedef struct
{
	const char* name;
	GLenum numLight; // GL_LIGHT0, GL_LIGHT1...
	int enabled;
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float position[3];
	float intensity;
} Light;

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
	float opacity; // indice de opacidad/transparencia

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
