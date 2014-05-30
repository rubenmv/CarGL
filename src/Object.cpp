#include "Object.h"

#include "tiny_obj_loader.h"
#include "Material.h"

#include "GL/gl.h"

#include <iostream>
#include <fstream>

Object::Object(const char* fileName, float position[3])
{
    material = 0;

    this->position[0] = position[0];
    this->position[1] = position[1];
    this->position[2] = position[2];

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

	std::vector<Object::shape_t> tempModel;
	// Carga el modelo OBJ con el tiny_obj_loader al modelo del recurso
	// Parametros: Modelo donde se guarda, direccion/filename del obj, basepath para el mtl
	std::string err = tinyobj::LoadObj(tempModel, fileName, basePath);
	if (!err.empty())
	{
		std::cerr << "Error loading obj model: " << err << std::endl;
	}
	else
	{
		// Para hacerlo mas facil aseguramos que el modelo esta exportado como un mismo objecto todo
		// IMPORTANTE: Debemos asegurarnos que el modelo fue exportado todo como un mismo objeto
		this->model = tempModel[0];

		// Generamos la display list
		createDisplayList();

		material = new Material(this->model.material, false); // No transparente
	}

	tempModel.clear();
}

Object::~Object()
{
	if(material != 0)
	{
		delete material;
	}
    material = 0;
}

void Object::createDisplayList()
{
	// Create the id for the list
	id = glGenLists(1);

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	// start list
	glNewList(id, GL_COMPILE);

		glVertexPointer( 3, GL_FLOAT, 0, &model.mesh.positions[0] );	// Posiciones

		if ( !model.mesh.normals.empty() )
		{
			glNormalPointer( GL_FLOAT, 0, &model.mesh.normals[0] );		// Normales
		}

		if ( !model.mesh.texcoords.empty() )
		{
			glTexCoordPointer( 2, GL_FLOAT, 0, &model.mesh.texcoords[0] );	// uv
		}

		glDrawElements( GL_TRIANGLES, model.mesh.indices.size(), GL_UNSIGNED_INT, &model.mesh.indices[0] );

	// endList
	glEndList();

	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
}

void Object::draw()
{
	glPushMatrix();
		material->bind();

		glTranslated(position[0], position[1], position[2]);
		//glRotated(ry,0,1,0);//Actualizamos la rotacion
		//PushMatrix(); // ooo guardo la matriz q tengo en ese momento
		//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		//glColor4fv( colores[0] );
		glLoadName(0);  // No seleccionable
		glCallList(id);

		material->unbind();
	glPopMatrix();
}
