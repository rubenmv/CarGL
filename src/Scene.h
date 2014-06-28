#ifndef SCENE_H
#define SCENE_H

#include "Clock.h"
#include <vector>

#include "Global.h"

#include <GL/glu.h>
#include <GL/glui.h>

#define MAX_WHEEL_ROTATION 20
#define MAX_SPEED 0.15
#define ACCELERATION 0.005
#define FRICTION 0.98
#define RADIANS 0.017
#define WHEEL_ROTATION_X 180
#define WHEEL_ROTATION_Y 1

class Object;
class GuiManager;
class Texture;

class Scene {
public:
	static Scene* instance();
    virtual ~Scene();

    // Camara activa
    Camera* activeCamera;

	Object* objSeleccion; // Object al que pertenece la seleccion, este es el que transformamos
	Object* iconSelection; // Object del icono de seleccion encima que se ve encima del objeto

    // Flags para activar/desactivar caracteristicas
    int ambientLighting;
	int wireframe;
	int textures;
	int mipmapping;
	int culling;
	int zbuffer;
	int smooth_shading;
	int perspective; // indica si se usa perspectiva o paralela
	int clockwise; // Sentido de dibujado de las caras de los poligonos

	int show_reflections;
	int show_skybox;
	int show_car;
    int show_ruedas;
    int show_carretera;
    int show_acera;
    int show_rotonda;
    int show_farolas;
    int show_edificios;
    int show_cubos;
    int show_papeleras;
    int show_bancos;
    int show_senales;

	Vector3 view_rotation; // Esta para los accesos de teclado
    float view_rotate[16];
    float view_position[3];
    float scale;

	void reshape(int x, int y);
	void pick3D(int mouse_x, int mouse_y);
	// Una vez realizado el pick3D se busca el objeto seleccionado
	void setSelection(int seleccion);
    void initOpenGL();

    // Agrega una camara e indica si es la activa
    void addCamera( const char* name, float px, float py, float pz, float lx, float ly, float lz, bool tracing = false, bool active = false );
    void addLight( const char* name, GLenum numLight, int enabled, float position[3], float intensity, float ambient[4], float diffuse[4], float specular[4] );
    // Agrega objetos a la escena e inicializa otras cosas
    void initObjects();
    // Bucle de dibujado de la escena
    void render();

    // Indica el sentido en el que gira el eje, 1 = derecha, -1 = izquierda, 0 = no hay giro
	int wheelsRotation;
    // Rotacion a la que se quiere llegar en este momento, es el targetRotation del objeto (coche) seleccionado
    float carRotation;
    float carSpeed;
    // El coche solo gira si se esta moviendo
    int moving; // 1 acelera, -1 desacelera

    Vector3 direction;

    GuiManager* guiManager;

    // Activar la camara indicada
    void setCamera(int id);

	// Busca si un objeto existe con su filename y devuelve su primera ID de displaylist
    unsigned int getObjectList(std::string fileName);

    // Devuelve una textura ya cargada (la crea si es necesario)
	Texture* getTexture(std::string fileName);

private:
	Scene();
	static Scene* pInstance;

	enum SkyboxEnum {
		SKYFRONT = 0,
		SKYBACK,
		SKYLEFT,
		SKYRIGHT,
		SKYUP,
		SKYDOWN
	};
	Texture* SkyboxTexture[6];

	// Objetos de la escena
	std::vector<Object*> objects;
	Object* road; // La carretera se renderiza por separado para aplicar reflejo

    // Camaras en la escena
    std::vector<Camera*> cameras;
     // Luces en la escena
    std::vector<Light*> lights;
    // Guarda las texturas ya cargadas para que los materiales las compartan
	std::vector<Texture*> loadedTextures;

	int seleccion; // Parte del objeto seleccionado

	// Normaliza un vector
	Vector3 normalize(Vector3 v);
	// Comprueba si un valor float esta dentro de un rango indicado con +-=range
	bool inRange(float value, float range);

	// Proyecciones
    void setPerspective();
	void setParallel();
	double projFar; // Necesitamos actualizarlo con el escalado

	float aspectRatio;

	// Para el calculo de FPS
	int frame, actualTime,timebase;
	Clock timerClock;

	void initRender();
	void renderSkybox();
	void renderReflection();
	void renderLights();
	void renderCamera();
	void renderObjects();
	void updateObjects();
};

#endif // SCENE_H
