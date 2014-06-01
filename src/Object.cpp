#include "Object.h"

#include "tiny_obj_loader.h"
#include "Material.h"

#include "GL/gl.h"

#include <iostream>
#include <fstream>

Object::Object(const char* fileName, bool transparent /* = false */)
{
	this->fileName = std::string(fileName);

	this->transparent = transparent;

    this->position[0] = 0;
    this->position[1] = 0;
    this->position[2] = 0;
    this->rotation[0] = 0;
    this->rotation[1] = 0;
    this->rotation[2] = 0;

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
	std::string err = tinyobj::LoadObj(this->model, fileName, basePath);

	if (!err.empty())
	{
		std::cerr << "Error loading obj model: " << err << std::endl;
	}
	else
	{
		// Generamos la display list
		createDisplayList();

		for (size_t i = 0; i < model.size(); ++i)
		{
			materials.push_back(new Material(this->model[0].material, transparent)); // No transparente
		}
	}

	timer.start();
}

Object::~Object()
{
	if(!materials.empty())
	{
		for (size_t i = 0; i < model.size(); ++i)
		{
			delete materials.back(); // No transparente
			materials.pop_back();
		}
	}
}

void Object::createDisplayList()
{
	// Creamos las listas necesarias para todos los objetos del modelo
	displayLists = glGenLists(model.size());

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	// Rellenamos cada display list creada
	for (size_t i = 0; i < model.size(); ++i)
	{
		// start list
		glNewList(displayLists+i, GL_COMPILE);


		if ( !model[i].mesh.positions.empty() )
		{
			glVertexPointer( 3, GL_FLOAT, 0, &model[i].mesh.positions[0] );	// Posiciones

			if ( !model[i].mesh.normals.empty() )
			{
				glNormalPointer( GL_FLOAT, 0, &model[i].mesh.normals[0] );		// Normales
			}

			if ( !model[i].mesh.texcoords.empty() )
			{
				glTexCoordPointer( 2, GL_FLOAT, 0, &model[i].mesh.texcoords[0] );	// uv
			}

			glDrawElements( GL_TRIANGLES, model[i].mesh.indices.size(), GL_UNSIGNED_INT, &model[i].mesh.indices[0] );
		}

		// endList
		glEndList();

		// Una vez creada la lista podemos limpiar la informacion de vertices
		model[i].mesh.positions.clear();
		model[i].mesh.normals.clear();
		model[i].mesh.texcoords.clear();
		model[i].mesh.indices.clear();
	}

	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
}

void Object::draw()
{
	for (size_t i = 0; i < model.size(); ++i)
	{
		glPushMatrix();
			materials[i]->bind();

			glTranslated(position[0], position[1], position[2]);
			if (constantRotation)
			{
				glRotatef(rotation[0] * timer.getTicks(), 1.0, 0.0, 0.0);
				glRotatef(rotation[1] * timer.getTicks(), 0.0, 1.0, 0.0);
				glRotatef(rotation[2] * timer.getTicks(), 0.0, 0.0, 1.0);
			}

			glLoadName(0);
			glCallList(displayLists+i);

			materials[i]->unbind();
		glPopMatrix();
	}
}

void Object::setConstantRotation(short int x, short int y, short int z, float rotation /* = 0.0 */ )
{
	if (rotation == 0.0)
	{
		constantRotation = false;
		this->rotation[0] = rotation;
		this->rotation[1] = rotation;
		this->rotation[2] = rotation;
	}
	else
	{
		if ( x == 1 )
		{
			this->rotation[0] = rotation;
			constantRotation = true;
		}
		if ( y == 1 )
		{
			this->rotation[1] = rotation;
			constantRotation = true;
		}
		if ( z == 1 )
		{
			this->rotation[2] = rotation;
			constantRotation = true;
		}
	}
}
