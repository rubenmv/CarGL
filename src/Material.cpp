#include "Material.h"
#include "Texture.h"

#include "Scene.h"
#include <GL/gl.h>

Material::Material(material_t material, bool transparent)
{
	scene = Scene::instance();
    texture = 0;

    this->transparent = transparent;
	this->material = material;
	this->name = material.name;
	// Textura
	if(!material.diffuse_texname.empty())
	{
		texture = scene->getTexture(material.diffuse_texname);
	}
}

Material::~Material()
{
    texture = 0;
    scene = 0;
}

void Material::bind()
{
	// Preparamos el color a aplicar
	float color[4] = { 1.0, 1.0, 1.0, 1.0 }; // Por defecto para los que tienen texturas
	if (transparent)
	{
		// Para objetos semitransparentes debemos desactivar el culling siempre
		// ya que tendriamos que poder ver las caras interiores
		glDisable(GL_CULL_FACE);
		// Como es SEMI transparencia queremos que la iluminacion tambien
		// le afecte a los objetos de este tipo
		//glDisable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		// Debemos de asegurarnos que la opacidad es menor de 1 en el .mtl
		color[3] = material.opacity; // Alpha para transparencia
	}

	// Dibujamos texturas con color de material por defecto (no queremos que el color afecte a la textura)
	if ( scene->textures && texture != 0)
	{
		texture->bind();
	}
	// Dibujamos materiales con su color
	else
	{
		color[0] = material.diffuse[0];
		color[1] = material.diffuse[1];
		color[2] = material.diffuse[2];
	}

	glColor4f(color[0], color[1], color[2], color[3]);
	//glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR,  material.specular);
	glMaterialf(GL_FRONT, GL_SHININESS,	 material.shininess);

}

void Material::unbind()
{
	// Reseteamos el color para que no afecte a lo que no debe
	glColor4f(1.0, 1.0, 1.0, 1.0);
	// Asi se evita que modelos sin textura apliquen la ultima textura
	if (texture != 0)
	{
		texture->unbind();
	}

	if (transparent)
	{
		glDisable(GL_BLEND);

		// Reactivamos el resto de propiedades segun la escena
		if(scene->culling)
		{
			glEnable(GL_CULL_FACE);
		}

		if(scene->ambientLighting)
		{
			glEnable(GL_LIGHTING);
		}
	}
}

void Material::setColor(float color[3])
{
	material.diffuse[0] = color[0];
	material.diffuse[1] = color[1];
	material.diffuse[2] = color[2];
}

