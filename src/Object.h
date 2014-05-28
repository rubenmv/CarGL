#ifndef OBJECT_H
#define OBJECT_H

class Material;

class Object {
public:
    Object();
    virtual ~Object();
private:
    Material* material; // Este contiene la textura
};

#endif // OBJECT_H
