#ifndef OBJECT_H
#define OBJECT_H

#include <vector>

#include "Clock.h"
#include "Global.h"

class Material;

class Object
{
public:
	ID id;

    Object(const char* fileName, ID id, Vector3 position = Vector3(), Vector3 rotation = Vector3(), bool selectable = false, bool transparent = false);
    virtual ~Object();

    std::string fileName;

    std::vector<shape_t> model;

	Vector3 position;
    Vector3 rotation;
    // Establece una rotacion constante en los ejes indicados (valores 0,1)
    void setConstantRotation(short int x, short int y, short int z, float rotation = 0.0);

    void draw();

    bool selectable;

private:
    std::vector<Material*> materials; // Este contiene la textura
    // IDs de la display list de las mallas que contiene el modelo/objeto
    // En realidad guarda la primera lista y luego se mueve en las consecutivas, si las hay
    unsigned int displayLists;
    // Genera la display list para este objecto
    void createDisplayList();

    bool constantRotation;

    Clock timer;

    bool transparent;
};

#endif // OBJECT_H
