#include "Texture.h"

#include <GL/glu.h>
#include <iostream>
#include "SOIL.h" // Para cargar imagenes, no solo jpeg

Texture::Texture(std::string filePath, bool linear)
{
    fileName = filePath;

    unsigned char* image = SOIL_load_image(filePath.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);

    if( 0 == image )
	{
		std::cout << "SOIL loading error: " << SOIL_last_result() << std::endl;
		std::cout << "Error cargando imagen: " << filePath << std::endl << std::endl;
	}

	glEnable( GL_TEXTURE_2D );
    // Generamos la textura en OpenGL
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable( GL_TEXTURE_2D );

	// La info de pixeles de la imagen ya no es necesaria
    delete image;
}

Texture::~Texture()
{
    glDeleteTextures(1, &textureID);
}

void Texture::bind()
{
    //glEnable( GL_TEXTURE_2D );
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	//glDisable( GL_TEXTURE_2D );
}
