#include "Object.h"

#include "tiny_obj_loader.h"
#include "Material.h"

#include "GL/gl.h"

#include <iostream>
#include <fstream>
#include <string.h>

Object::Object(const char* fileName, ID id, Vector3 position, Vector3 rotation, bool selectable /* = false */, bool transparent /*= false*/)
{
	this->fileName = std::string(fileName);
	this->id = id;
	this->transparent = transparent;

	this->position = position;
	this->rotation = rotation;

	this->selectable = selectable;

    constantRotation = false;

	// Carga el modelo con el tiny_obj_loader
	std::ifstream fileIn(fileName);

	// Path base del modelo donde encontrar el mtl
	std::string path = fileName;
	std::string bp;
	// Extraemos el path
	for (int i = path.size()-1 ; i > 0; i--)
	{
		if (path.at(i) == '/')
		{
			bp = path.substr(0, i+1);
			break;
		}
	}
	// Y de nuevo a const char*
	const char* basePath = bp.c_str();

	// Carga el modelo OBJ con el tiny_obj_loader al modelo del recurso
	// Parametros: Modelo donde se guarda, direccion/filename del obj, basepath para el mtl
	std::string err = tinyobj::LoadObj(this->shapes, fileName, basePath);

	if (!err.empty())
	{
		std::cerr << "Error loading obj model: " << err << std::endl;
	}
	else
	{
		// Se va a crear una lista por modelo/malla
		dListCount = shapes.size();
		// Generamos la display list
		createDisplayList();

		for (size_t i = 0; i < shapes.size(); ++i)
		{
			materials.push_back(new Material(this->shapes[i].material, transparent)); // No transparente
		}

		// Como es un coche y puede cambiar el color de la carroceria
		// recogemos el color que traia por defecto
		if ( id == COCHE )
		{
			color[0] = this->shapes[0].material.diffuse[0];
			color[1] = this->shapes[0].material.diffuse[1];
			color[2] = this->shapes[0].material.diffuse[2];
		}

	}

	timer.start();
}

Object::~Object()
{
	if(!materials.empty())
	{
		for (size_t i = 0; i < shapes.size(); ++i)
		{
			delete materials.back(); // No transparente
			materials.pop_back();
		}
	}

	glDeleteLists(firstDList, dListCount);
}

void Object::createDisplayList()
{
	// Creamos las listas necesarias para todos los objetos del modelo
	firstDList = glGenLists(dListCount);

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	// Rellenamos cada display list creada
	for (size_t i = 0; i < dListCount; ++i)
	{
		// start list
		glNewList(firstDList+i, GL_COMPILE);

		if ( !shapes[i].mesh.positions.empty() )
		{
			glVertexPointer( 3, GL_FLOAT, 0, &shapes[i].mesh.positions[0] );	// Posiciones

			if ( !shapes[i].mesh.normals.empty() )
			{
				glNormalPointer( GL_FLOAT, 0, &shapes[i].mesh.normals[0] );		// Normales
			}

			if ( !shapes[i].mesh.texcoords.empty() )
			{
				glTexCoordPointer( 2, GL_FLOAT, 0, &shapes[i].mesh.texcoords[0] );	// uv
			}

			glDrawElements( GL_TRIANGLES, shapes[i].mesh.indices.size(), GL_UNSIGNED_INT, &shapes[i].mesh.indices[0] );
		}

		// endList
		glEndList();

		// Una vez creada la lista podemos limpiar la informacion de vertices
		shapes[i].mesh.positions.clear();
		shapes[i].mesh.normals.clear();
		shapes[i].mesh.texcoords.clear();
		shapes[i].mesh.indices.clear();
	}

	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
}

void Object::draw()
{
	// Dibujamos todas las display list
	for (size_t i = 0; i < dListCount; ++i)
	{
		glPushMatrix();
			if (id == COCHE) // Los coches puede cambiar su color de carroceria
			{
				materials[i]->setColor(color);
			}

			materials[i]->bind();

			// Las transformaciones se aplican localmente y no son conmutativas, primero trasladamos, luego rotamos
			glTranslatef(position.x, position.y, position.z);

			if (constantRotation)
			{
				glRotatef(rotation.x * timer.getTicks(), 1.0, 0.0, 0.0);
				glRotatef(rotation.y * timer.getTicks(), 0.0, 1.0, 0.0);
				glRotatef(rotation.z * timer.getTicks(), 0.0, 0.0, 1.0);
			}
			else
			{
				glRotatef(rotation.x, 1, 0, 0);
				glRotatef(rotation.y, 0, 1, 0);
				glRotatef(rotation.z, 0, 0, 1);
			}

			if ( selectable )
			{
				glLoadName(firstDList+i);
			}
			else
			{
				glLoadName(0);
			}

			glCallList(firstDList+i);

			materials[i]->unbind();
		glPopMatrix();
	}
}

void Object::setConstantRotation(short int x, short int y, short int z, float rotation /* = 0.0 */ )
{
	if (rotation == 0.0)
	{
		constantRotation = false;
		this->rotation.x = rotation;
		this->rotation.y = rotation;
		this->rotation.z = rotation;
	}
	else
	{
		if ( x == 1 )
		{
			this->rotation.x = rotation;
			constantRotation = true;
		}
		if ( y == 1 )
		{
			this->rotation.y = rotation;
			constantRotation = true;
		}
		if ( z == 1 )
		{
			this->rotation.z = rotation;
			constantRotation = true;
		}
	}
}
