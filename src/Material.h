#ifndef MATERIAL_H
#define MATERIAL_H

#include "Object.h"

class Texture;
class Scene;

class Material
{
public:
    Material(material_t material, bool transparent);
    virtual ~Material();

    bool transparent;

    void bind();
    void unbind();

    void setColor(float color[3]);
private:
	material_t material; // Este ya tiene toda la info del material
    Texture* texture;

    Scene* scene; // Para comprobar el estado de show_textures
};

#endif // MATERIAL_H
