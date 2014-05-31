#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <ctime>

class Object;

class Scene {
public:
    struct Camera
    {
        float position[3];
        float lookAt[3];
    };

    struct Light
    {
    	bool active;
        float position[3];
        float color[3];
        float intensity;
    };

    Scene();
    virtual ~Scene();

	void reshape(int x, int y);

    void initOpenGL();

    // Agrega una camara e indica si es la activa
    void addCamera( float px, float py, float pz, float lx, float ly, float lz, bool active = false );
    // Agrega objetos a la escena e inicializa otras cosas
    void initObjects();
    // Bucle de dibujado de la escena
    void render();
private:
	// Objetos de la escena
	std::vector<Object*> objects;

    // Camaras en la escena
    std::vector<Camera*> cameras;
    // Camara activa
    Camera* activeCamera;

     // Luces en la escena
    std::vector<Light*> lights;

	// Proyecciones
    void setPerspective();
	void setParallel();

	bool perspective; // indica si se usa perspectiva o paralela
	float aspectRatio;

	std::clock_t timerClock;
};

#endif // SCENE_H
