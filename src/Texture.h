#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture {
public:
    Texture(std::string filePath);
    virtual ~Texture();

    std::string fileName;

    void bind();
    void unbind();
private:
    unsigned int textureID;
    unsigned int mipmapID;

    int width;
    int height;
    int channels;

	// Indica si la textura se ha de bindear
	bool binding;
	// Indica si se aplica suavizado linear
	bool linear;
	// Indica si la textura se repite
	bool repeat;
};

#endif // TEXTURE_H
