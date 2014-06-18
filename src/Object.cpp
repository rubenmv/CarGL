#include "Object.h"

#include "tiny_obj_loader.h"
#include "Material.h"

#include "GL/gl.h"
#include "Scene.h"

#include <iostream>
#include <fstream>
#include <string.h>

Object::Object(	const char* fileName, ID id, Vector3 position, Vector3 rotation,
				Object* parent, bool selectable, bool transparent)
{
	this->fileName = std::string(fileName);
	this->id = id;
	this->parent = parent;
	this->transparent = transparent;

	firstDList = 0;

	// Si tiene parent la posicion es la relativa a este
	if ( parent != 0 )
	{
		relativePosition = position;
	}
	else
	{
		this->position = position;
	}

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
		// Generamos las display lists para cada shape
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

	parent = 0;
}

void Object::createDisplayList()
{
	// Queremos que los seleccionables tengan listas unicas
	if ( !this->selectable )
	{
		// Antes de generar nuevas listas, preguntamos a la escena si ya
		// existe un objeto igual y recogemos su primer id de display list
		firstDList = Scene::instance()->getObjectList( this->fileName );
	}

	// Si no se ha encontrado el objeto, generamos nuevas display lists
	if ( firstDList == 0 )
	{
		// Creamos las listas necesarias para todos los objetos del modelo
		// Esto se debe a que el modelo se exporta dividido en distintas partes
		// cada una con sus materiales y texturas
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
		}

		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		glDisableClientState( GL_NORMAL_ARRAY );
		glDisableClientState( GL_VERTEX_ARRAY );
	}

	// Una vez creada/obtenida la lista podemos limpiar la informacion de vertices de cada shape
	for (size_t i = 0; i < shapes.size(); ++i)
	{
		shapes[i].mesh.positions.clear();
		shapes[i].mesh.normals.clear();
		shapes[i].mesh.texcoords.clear();
		shapes[i].mesh.indices.clear();
	}
}

void Object::draw()
{
	// Dibujamos todas las display list
	for (size_t i = 0; i < dListCount; ++i)
	{
		glPushMatrix();
			// Los coches puede cambiar su color de carroceria
			// pero el resto de colores se mantiene
			if (id == COCHE && materials[i]->name.compare("MaterialCarroceria") == 0 )
			{
				materials[i]->setColor(color);
			}

			materials[i]->bind();

			// Si tiene parent la posicion siempre es relativa a este
			if ( parent != 0 )
			{
				// Aplicamos la rotacion con respecto al padre
				glTranslatef(parent->position.x, parent->position.y, parent->position.z);

				glRotatef(parent->rotation.y, 0.0, 1.0, 0.0);
				glRotatef(parent->rotation.x, 1.0, 0.0, 0.0);
				glRotatef(parent->rotation.z, 0.0, 0.0, 1.0);

				// Una vez estamos rotados sobre el padre, movemos a la posicion relativa con respecto a este
				this->position.x = relativePosition.x;
				this->position.y = relativePosition.y;
				this->position.z = relativePosition.z;
			}
			// Movemos a la posicion relativa
			glTranslatef(position.x, position.y, position.z);

			// Rotacion local
			if (constantRotation)
			{
				glRotatef(rotation.y * timer.getTicks(), 0.0, 1.0, 0.0);
				glRotatef(rotation.x * timer.getTicks(), 1.0, 0.0, 0.0);
				glRotatef(rotation.z * timer.getTicks(), 0.0, 0.0, 1.0);
			}
			else
			{
				glRotatef(rotation.y, 0, 1, 0);
				glRotatef(rotation.x, 1, 0, 0);
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

void Object::setTransparent(bool transparent)
{
	for (size_t i = 0; i < materials.size(); ++i)
	{
		materials[i]->transparent = transparent;
	}
}
