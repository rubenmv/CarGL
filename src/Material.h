#ifndef MATERIAL_H
#define MATERIAL_H

#include "Object.h"

class Texture;
class Scene;

class Material
{
public:
	std::string name;
    Material(material_t material, bool transparent);
    virtual ~Material();

    bool transparent;

    void bind();
    void unbind();

    void setColor(float color[3]);

    material_t material; // Mirar contenido en Global.h
private:

    Texture* texture;

    Scene* scene; // Para comprobar el estado de show_textures
};

#endif // MATERIAL_H
