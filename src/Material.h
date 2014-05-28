#ifndef MATERIAL_H
#define MATERIAL_H

class Texture;

class Material {
public:
    Material();
    virtual ~Material();
private:
    Texture* texture;
};

#endif // MATERIAL_H
