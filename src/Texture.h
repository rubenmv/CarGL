#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture {
public:
    Texture(std::string filePath, bool linear, bool repeat);
    virtual ~Texture();

    void bind();
    void unbind();
private:
    std::string fileName;
    unsigned int textureID;

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
