#ifndef MATERIAL_H
#define MATERIAL_H

#include "Object.h"

class Texture;

class Material
{
public:
    Material(Object::material_t material, bool transparent);
    virtual ~Material();

    void bind();
    void unbind();
private:
	Object::material_t material; // Este ya tiene toda la info del material
    Texture* texture;
    bool transparent;
};

#endif // MATERIAL_H
