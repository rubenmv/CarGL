#include "Material.h"
#include "Texture.h"

#include <GL/gl.h>

Material::Material(material_t material, bool transparent)
{
    texture = 0;

    this->transparent = transparent;
	this->material = material;
	// Textura
	if(!material.diffuse_texname.empty())
	{
		texture = new Texture(material.diffuse_texname, false);
	}
}

Material::~Material()
{
    texture = 0;
}

void Material::bind()
{
	if (transparent)
	{
		glEnable (GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	}

	if (texture != 0)
	{
		texture->bind();
	}

	// Solo necesitamos la componente alpha para el diffuse
	float diffuse[4] = { material.diffuse[0], material.diffuse[1], material.diffuse[2], 0.5  };
	// Con GL_COLOR_MATERIAL activado se podria utilizar glColor3f para aplicar ambient y diffuse
	// al mismo tiempo, esto puede ser mas rapido pero no se pueden aplicar ambient y diffuse por separado
	//glColor3f(material.ambient[0], material.ambient[1], material.ambient[2]);
	glMaterialfv(GL_FRONT, GL_AMBIENT,   material.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  material.specular);
    glMaterialf(GL_FRONT, GL_SHININESS,	 material.shininess);
}

void Material::unbind()
{
	// Asi se evita que modelos sin textura apliquen la ultima textura
	if (texture != 0)
	{
		texture->unbind();
	}

	if (transparent)
	{
		glDisable (GL_BLEND);
	}
}
