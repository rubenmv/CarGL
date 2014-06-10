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

    Object(	const char* fileName, ID id, Vector3 position = Vector3(), Vector3 rotation = Vector3(),
			Object* parent = 0, bool selectable = false, bool transparent = false);
    virtual ~Object();

    std::string fileName;
    std::string name; // nombre a mostrar en el GLUI para coches, camaras, etc.

    std::vector<shape_t> shapes;

    float color[3]; // Nos sirve para acceder desde GLUI

	Vector3 position;
    Vector3 rotation;
    // Establece una rotacion constante en los ejes indicados (valores 0,1)
    void setConstantRotation(short int x, short int y, short int z, float rotation = 0.0);

    void draw();

    bool selectable;

    // Si el objeto se compone de distintas mallas separadas, guardamos la primera display list
    // y el total de listas que contiene, una por malla cada una con su material separado
    unsigned int firstDList;
    unsigned int dListCount;

    // Si tiene parent la posicion se convierte en relativa con respecto a este
    Object* parent;

    void setTransparent(bool transparent);

	// Rotacion a la que se quiere llegar, esto impide controlar que los coches giren
	// cuando no estan avanzando
    Vector3 targetRotation;

private:
    std::vector<Material*> materials; // Este contiene la textura
    // Genera la display list para este objecto
    void createDisplayList();

    bool constantRotation;

    Clock timer;

    bool transparent;

    Vector3 relativePosition;
};

#endif // OBJECT_H
