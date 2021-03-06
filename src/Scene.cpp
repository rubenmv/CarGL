#include "Scene.h"
#include "Object.h"
#include "Texture.h"

#include "GuiManager.h"
#include <iostream>
#include <math.h>
#include <time.h>  // Para random

Scene* Scene::pInstance = 0;

static const char* OBJ_CARRETERA		= "assets/carretera/carretera.obj";
static const char* OBJ_ACERA			= "assets/acera/acera.obj";
static const char* OBJ_COCHE			= "assets/cart/cart.obj";
static const char* OBJ_RUEDA_T1			= "assets/cart/rueda_t1.obj";
static const char* OBJ_RUEDA_T2			= "assets/cart/rueda_t2.obj";
static const char* OBJ_VOLANTE			= "assets/cart/volante.obj";
static const char* OBJ_FAROLA			= "assets/farola/farola.obj";
static const char* OBJ_BANCO			= "assets/banco/banco.obj";
static const char* OBJ_PAPELERA			= "assets/papelera/papelera.obj";
static const char* OBJ_EDIFICIO_1		= "assets/edificios/edificio1.obj";
static const char* OBJ_EDIFICIO_2		= "assets/edificios/edificio2.obj";
static const char* OBJ_SENAL_TRAFICO	= "assets/senal_trafico/senal_trafico.obj";
static const char* OBJ_ROTONDA_BASE		= "assets/rotonda/rotonda_base.obj";
static const char* OBJ_ROTONDA_BOLA		= "assets/rotonda/rotonda_bola.obj";
static const char* OBJ_SELECCION		= "assets/seleccion/seleccion.obj";
static const char* OBJ_LUNA_COCHE		= "assets/cart/luna.obj";

float view_rotate_c[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float view_position_c[3] = { 0.0, -2.0, -9.0 };

#define NUM_EDF_BLOQUE 9
#define NUM_BLOQUES 4
// Posicion de los edficios del bloque con x,y positivo
float pos_edf[NUM_EDF_BLOQUE*NUM_BLOQUES][3] = {
	// Bloque 1
	{40.0, 0.0, 10.0},	{25.0, 0.0, 10.0},	{12.0, 0.0, 12.0},	{10.0, 0.0, 25.0},	{10.0, 0.0, 40.0},
	{40.0, 0.0, 25.0},	{40.0, 0.0, 40.0},	{25.0, 0.0, 40.0},	{25.0, 0.0, 25.0},
	// Bloque 2
	{-40.0, 0.0, 10.0},	{-25.0, 0.0, 10.0},	{-12.0, 0.0, 12.0},	{-10.0, 0.0, 25.0},	{-10.0, 0.0, 40.0},
	{-40.0, 0.0, 25.0},	{-40.0, 0.0, 40.0},	{-25.0, 0.0, 40.0},	{-25.0, 0.0, 25.0},
	// Bloque 3
	{-40.0, 0.0, -10.0}, {-25.0, 0.0, -10.0}, {-12.0, 0.0, -12.0}, {-10.0, 0.0, -25.0}, {-10.0, 0.0, -40.0},
	{-40.0, 0.0, -25.0}, {-40.0, 0.0, -40.0}, {-25.0, 0.0, -40.0}, {-25.0, 0.0, -25.0},
	// Bloque 4
	{40.0, 0.0, -10.0},	{25.0, 0.0, -10.0},	{12.0, 0.0, -12.0},	{10.0, 0.0, -25.0},	{10.0, 0.0, -40.0},
	{40.0, 0.0, -25.0},	{40.0, 0.0, -40.0},	{25.0, 0.0, -40.0},	{25.0, 0.0, -25.0}
};

float rot_edf[NUM_EDF_BLOQUE*NUM_BLOQUES][3] = {
	// Bloque 1
	{0.0, 180.0, 0.0},	{0.0, 180.0, 0.0},	{0.0, -135.0, 0.0},	{0.0, -90.0, 0.0},	{0.0, -90.0, 0.0},
	{0.0, 90.0, 0.0},	{0.0, 90.0, 0.0},	{0.0, 0.0, 0.0},	{0.0, 180.0, 0.0},
	// Bloque 2
	{0.0, 180.0, 0.0},	{0.0, 180.0, 0.0},	{0.0, 135.0, 0.0},	{0.0, 90.0, 0.0},	{0.0, 90.0, 0.0},
	{0.0, -90.0, 0.0},	{0.0, -90.0, 0.0},	{0.0, 0.0, 0.0},	{0.0, 180.0, 0.0},
	// Bloque 3
	{0.0, 0.0, 0.0},	{0.0, 0.0, 0.0},	{0.0, 45.0, 0.0},	{0.0, 90.0, 0.0},	{0.0, 90.0, 0.0},
	{0.0, -90.0, 0.0},	{0.0, -90.0, 0.0},	{0.0, 180.0, 0.0},	{0.0, 0.0, 0.0},
	// Bloque 4
	{0.0, 0.0, 0.0},	{0.0, 0.0, 0.0},	{0.0, -45.0, 0.0},	{0.0, -90.0, 0.0},	{0.0, -90.0, 0.0},
	{0.0, 90.0, 0.0},	{0.0, 90.0, 0.0},	{0.0, 180.0, 0.0},	{0.0, 0.0, 0.0}
};

// Inicializacion del singleton
Scene* Scene::instance() {
   if (pInstance == 0) {
      pInstance = new Scene;
   }
   return pInstance;
}

Scene::Scene()
{
	guiManager = 0;

    activeCamera = 0;

    aspectRatio = 1.0;

    initOpenGL();

    timerClock.start();

	// FLAGS
	ambientLighting = 1;
    textures = 1;
    mipmapping = 1;
    wireframe = 0;
    culling = 1;
    zbuffer = 1;
	smooth_shading = 1;
	perspective = 1;
	clockwise = 0;

	show_reflections = 0;
	show_skybox = 1;
    show_car = 1;
    show_ruedas = 1;
    show_carretera = 1;
    show_acera = 1;
    show_rotonda = 1;
    show_farolas = 1;
    show_edificios = 1;
    show_cubos = 1;
    show_bancos = 1;
    show_senales = 1;
    show_papeleras = 1;

    memcpy(view_rotate, view_rotate_c, 16*sizeof(float));
    memcpy(view_position, view_position_c, 3*sizeof(float));
    scale = 1.0f;

    seleccion = 0;

    carRotation = 0.0;
    moving = 0;

    carSpeed = 0.0;

    frame = 0;
    actualTime = 0;
    timebase = 0;

    projFar = 1000; // Por defecto, luego se aplica con escala

    srand (time(NULL)); // Para poner edificios aleatorios
}

void Scene::initOpenGL()
{
	// Algunas de estas propiedades se cambiaran en el renderObjects()
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );

	glClearDepth(1.f);

    glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// Activa la iluminacion
    glEnable(GL_LIGHTING);
    // Luz ambiental de la escana
    GLfloat luzAmbiente[4] = {0.3, 0.3, 0.3, 1.0}; // Por defecto estaban a 0.2
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
    // El resto de luces se agregan con addLight

    // Normalizacion al escalar
	glEnable(GL_NORMALIZE);

    // Por defecto proyeccion perspectiva
    setPerspective();

    // Info sobre OpenGL
    std::cout << "OpenGL\nVersion: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
}

Scene::~Scene()
{
    while (!cameras.empty())
    {
        delete cameras.back();
        cameras.pop_back();
    }

    while (!lights.empty())
    {
        delete lights.back();
        lights.pop_back();
    }

	while (!loadedTextures.empty())
    {
        delete loadedTextures.back();
        loadedTextures.pop_back();
    }

    while (!objects.empty())
    {
        delete objects.back();
        objects.pop_back();
    }
    delete road;
	road = 0;

    activeCamera = 0;
    guiManager = 0;
}

void Scene::initObjects()
{
	Object* object = 0;

	float aceraHeight = 0.15; // Para los objetos encima de la acera

	// CARRETERA, se guarda por separado para aplicar reflejos
	road = new Object(OBJ_CARRETERA, CARRETERA, Vector3(), Vector3(), 0, false, true);

	// ACERAS
	object = new Object(OBJ_ACERA, ACERA, Vector3(-13.0, 0.0, -13.0));
	objects.push_back( object );
	object = new Object(OBJ_ACERA, ACERA, Vector3(13.0, 0.0, 13.0), Vector3(0.0, 180.0, 0.0) );
	objects.push_back( object );
	object = new Object(OBJ_ACERA, ACERA, Vector3(13.0, 0.0, -13.0), Vector3(0.0, -90.0, 0.0) );
	objects.push_back( object );
	object = new Object(OBJ_ACERA, ACERA, Vector3(-13.0, 0.0, 13.0), Vector3(0.0, 90.0, 0.0) );
	objects.push_back( object );


	// COCHE 1
	object = new Object(OBJ_COCHE, COCHE,
						Vector3(-0.9, 0.04, -10), Vector3(), 0, true ); // Seleccionable
	object->name = "Coche 1";
	// Le damos un color inicial diferente para que se distingan los coches
	object->color[0] = 0.5; object->color[1] = 0.3; object->color[2] = 0.4;
	objects.push_back( object );
	guiManager->addCarItem( object );

	objSeleccion = object; // Uso esto para pasarlo como parent a las ruedas y ya dejo uno seleccionado al final

	// Ruedas del coche 1, ponemos el coche como parent y las posiciones seran relativas a este
	object = new Object(OBJ_RUEDA_T2, RUEDA_D,
						Vector3(-0.45, 0.14, 1.5), Vector3(), objSeleccion);
	objects.push_back( object );
	object = new Object(OBJ_RUEDA_T2, RUEDA_T,
						Vector3(-0.44, 0.14, 0.25), Vector3(), objSeleccion);
	objects.push_back( object );

	object = new Object(OBJ_RUEDA_T1, RUEDA_D,
						Vector3(0.45, 0.14, 1.5), Vector3(), objSeleccion);
	objects.push_back( object );
	object = new Object(OBJ_RUEDA_T1, RUEDA_T,
						Vector3(0.44, 0.14, 0.25), Vector3(), objSeleccion);
	objects.push_back( object );

	// Volante 1 por separado, para que gire igual que las ruedas
	object = new Object(OBJ_VOLANTE, VOLANTE,
						Vector3(0.24, 0.77, 0.92), Vector3(45.0, 0.0, 0.0), objSeleccion);
	objects.push_back( object );

	// COCHE 2
	object = new Object(OBJ_COCHE, COCHE,
						Vector3(0.9, 0.04, -12.0), Vector3(), 0, true ); // Seleccionable
	object->name = "Coche 2";
	object->color[0] = 0.2; object->color[1] = 0.6; object->color[2] = 0.4;
	objects.push_back( object );
	guiManager->addCarItem( object );

	objSeleccion = object;

	// Ruedas del coche 2
	object = new Object(OBJ_RUEDA_T2, RUEDA_D,
						Vector3(-0.45, 0.14, 1.5), Vector3(), objSeleccion);
	objects.push_back( object );
	object = new Object(OBJ_RUEDA_T2, RUEDA_T,
						Vector3(-0.44, 0.14, 0.25), Vector3(), objSeleccion);
	objects.push_back( object );

	object = new Object(OBJ_RUEDA_T1, RUEDA_D,
						Vector3(0.45, 0.14, 1.5), Vector3(), objSeleccion);
	objects.push_back( object );
	object = new Object(OBJ_RUEDA_T1, RUEDA_T,
						Vector3(0.44, 0.14, 0.25), Vector3(), objSeleccion);
	objects.push_back( object );

	// Volante 2 por separado, para que gire igual que las ruedas
	object = new Object(OBJ_VOLANTE, VOLANTE,
						Vector3(0.24, 0.77, 0.92), Vector3(45.0, 0.0, 0.0), objSeleccion);
	objects.push_back( object );

	carRotation = objSeleccion->rotation.y;

	// FAROLAS, PAPELERAS, BANCOS
    float despX = 10;
    float despZ = 3;
    float despY = aceraHeight;
    int sign = -1;
    float rotation = 0;

    // En el eje x
    for(int j = 1; j < 5; j++)
	{
		for(int i = 1; i < 3; i++)
		{
			object = new Object(OBJ_FAROLA, FAROLA,
								Vector3(despX * j, despY, despZ * sign), Vector3(0.0, rotation, 0.0) );
			objects.push_back( object );

			if ( j % 2 == 0 )
			{
				object = new Object(OBJ_PAPELERA, PAPELERA,
								Vector3(2 + despX * j, despY, despZ * sign), Vector3(0.0, rotation, 0.0) );
				objects.push_back( object );

				object = new Object(OBJ_BANCO, BANCO,
								Vector3(despX * j - 5, despY, despZ * sign), Vector3(0.0, rotation, 0.0) );
				objects.push_back( object );
			}

			object = new Object(OBJ_FAROLA, FAROLA,
								Vector3(despX * -j, despY, despZ * sign), Vector3(0.0, rotation, 0.0) );
			objects.push_back( object );

			if ( j % 2 == 0 )
			{
				object = new Object(OBJ_PAPELERA, PAPELERA,
								Vector3(despX * -j - 2, despY, despZ * sign), Vector3(0.0, rotation, 0.0) );
				objects.push_back( object );

				object = new Object(OBJ_BANCO, BANCO,
								Vector3(despX * -j + 5, despY, despZ * sign), Vector3(0.0, rotation, 0.0) );
				objects.push_back( object );
			}

			sign *= -1;
			rotation = 180;
		}

		rotation = 0;
	}

	// En el eje z
	rotation = 90;
	for(int j = 1; j < 5; j++)
	{
		for(int i = 1; i < 3; i++)
		{
			object = new Object(OBJ_FAROLA, FAROLA,
								Vector3(despZ * sign, despY, despX * j), Vector3(0.0, rotation, 0.0) );
			objects.push_back( object );

			// Una papelera cada 2 farolas
			if ( j % 2 == 0 )
			{
				object = new Object(OBJ_PAPELERA, PAPELERA,
								Vector3(despZ * sign, despY, 2 + despX * j), Vector3(0.0, rotation, 0.0) );
				objects.push_back( object );

				object = new Object(OBJ_BANCO, BANCO,
								Vector3(despZ * sign, despY, despX * j - 5), Vector3(0.0, rotation, 0.0) );
				objects.push_back( object );
			}

			object = new Object(OBJ_FAROLA, FAROLA,
								Vector3(despZ * sign, despY, despX * -j), Vector3(0.0, rotation, 0.0) );
			objects.push_back( object );

			// Una papelera cada 2 farolas
			if ( j % 2 == 0 )
			{
				object = new Object(OBJ_PAPELERA, PAPELERA,
								Vector3(despZ * sign, despY, despX * -j - 2), Vector3(0.0, rotation, 0.0) );
				objects.push_back( object );

				object = new Object(OBJ_BANCO, BANCO,
								Vector3(despZ * sign, despY, 5 + despX * -j), Vector3(0.0, rotation, 0.0) );
				objects.push_back( object );
			}

			sign *= -1;
			rotation = -90;
		}
		rotation = 90;
	}

	// EDIFICIOS
	int randomNumber = 0;
	std::string edificio;
	float numEdificios = NUM_EDF_BLOQUE*NUM_BLOQUES;
	for(size_t i = 0; i < numEdificios; i++)
	{
		// El edificio se genera aleatoriamente
		randomNumber = rand() % 2;

		switch (randomNumber) {
			case 0:
				edificio = OBJ_EDIFICIO_1;
				break;
			case 1:
				edificio = OBJ_EDIFICIO_2;
				break;
			default:
				edificio = OBJ_EDIFICIO_1;
				break;
		}

		object = new Object(edificio.c_str(), EDIFICIO,
						Vector3(pos_edf[i][0], aceraHeight + pos_edf[i][1], pos_edf[i][2]),
						Vector3(rot_edf[i][0], rot_edf[i][1], rot_edf[i][2]) );
		objects.push_back( object );
	}

	// SENALES DE TRAFICO
	object = new Object(OBJ_SENAL_TRAFICO, SENAL,
						Vector3(9.0, aceraHeight, -2.5), Vector3(0.0, 90.0, 0.0) );
	objects.push_back( object );
	object = new Object(OBJ_SENAL_TRAFICO, SENAL,
						Vector3(-9.0, aceraHeight, 2.5), Vector3(0.0, -90.0, 0.0) );
	objects.push_back( object );
	object = new Object(OBJ_SENAL_TRAFICO, SENAL,
						Vector3(2.5, aceraHeight, 9.0), Vector3(0.0, 0.0, 0.0) );
	objects.push_back( object );
	object = new Object(OBJ_SENAL_TRAFICO, SENAL,
						Vector3(-2.5, aceraHeight, -9.0), Vector3(0.0, 180.0, 0.0) );
	objects.push_back( object );

	// ROTONDA, por separado para agregarle la rotacion y transparencia a la bola
	object = new Object(OBJ_ROTONDA_BASE, ROTONDA);
	objects.push_back( object );

	// Objetos transparentes al final para preservar la profuncdidad					  No seleccionable, transparente
	object = new Object(OBJ_ROTONDA_BOLA, ROTONDA, Vector3(), Vector3(), 0, false, true);
	object->setConstantRotation(0.0, 1.0, 0.0, 0.02);
	objects.push_back( object );
	// Icono de seleccion encima del objeto
	object = new Object(OBJ_SELECCION, SELECCION,
						Vector3(0.0, 2.0, 0.0), Vector3(), objSeleccion, false, true );// Su parent es el coche seleccionado actualmente
	object->setConstantRotation(0.0, 1.0, 0.0, 0.08);
	objects.push_back( object );

	iconSelection = object; // Icono de objeto seleccionado

	// Vamos a agregar los cristales a los coches, lo hacemos ahora porque los
	// objetos con transparencia deben dibujarse los ultimos
	int actualSize = objects.size();
	for(size_t i = 0; i < actualSize; i++)
	{
		if(objects[i]->id == COCHE)
		{
			object = new Object(OBJ_LUNA_COCHE, LUNA_COCHE,
							Vector3(), Vector3(), objects[i], false, true);
			objects.push_back(object);
		}
	}

	// Finalmente el skybox, solo cargamos las texturas
	// luego se dibujan sobre quads
	SkyboxTexture[SKYFRONT] = getTexture("assets/skybox/xpos.png");
	SkyboxTexture[SKYBACK] = getTexture("assets/skybox/xneg.png");
	SkyboxTexture[SKYLEFT] = getTexture("assets/skybox/zpos.png");
	SkyboxTexture[SKYRIGHT] = getTexture("assets/skybox/zneg.png");
	SkyboxTexture[SKYUP] = getTexture("assets/skybox/ypos.png");
	SkyboxTexture[SKYDOWN] = getTexture("assets/skybox/yneg.png");

}

void Scene::addCamera(const char* name, float px, float py, float pz, float lx, float ly, float lz, bool tracing /* = false */, bool active /* = false */)
{
    Camera* camera = new Camera();
    camera->name = name;
    camera->position.x = px;
    camera->position.y = py;
    camera->position.z = pz;
    camera->rotation.x = lx;
    camera->rotation.y = ly;
    camera->rotation.z = lz;
    camera->tracing = tracing;

    cameras.push_back(camera);

    if (active)
    {
        activeCamera = camera;
    }

    guiManager->addCameraItem(name, active);
}

void Scene::setCamera(int id)
{
	activeCamera = cameras[id];
	// Al final es view_position lo que actualizamos
	view_position[0] = activeCamera->position.x;
	view_position[1] = activeCamera->position.y;
	view_position[2] = activeCamera->position.z;
	view_rotation = activeCamera->rotation;
	memcpy(view_rotate, view_rotate_c, 16*sizeof(float));
	scale = 1.0f;
}

void Scene::addLight( const char* name, GLenum numLight, int enabled, float position[3], float intensity, float ambient[4], float diffuse[4], float specular[4] )
{
    Light* light = new Light();

	light->name = name;
    light->numLight = numLight;
    light->enabled = enabled;
    light->intensity = intensity; // La intensidad se aplicara durante el render

	memcpy(light->position, position, 3*sizeof(float));
	memcpy(light->ambient, ambient, 4*sizeof(float));
    memcpy(light->diffuse, diffuse, 4*sizeof(float));
    memcpy(light->specular, specular, 4*sizeof(float));

    glEnable(numLight);
    glLightfv(numLight, GL_AMBIENT,  light->ambient);
    glLightfv(numLight, GL_DIFFUSE,  light->diffuse);
    glLightfv(numLight, GL_SPECULAR, light->specular);

    glLightfv(numLight, GL_POSITION, light->position);

    lights.push_back(light);

    guiManager->addLightItem(light);
}

void Scene::setPerspective()
{
	perspective = true;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    gluPerspective(45, aspectRatio, 0.1, projFar*scale);
}

void Scene::setParallel()
{
	perspective = false;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-8, 8, -5, 5, 0.1, projFar*scale);
}

void Scene::reshape(int x, int y)
{
	int tx, ty, tw, th;
    GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
    glViewport( tx, ty, tw, th );

    aspectRatio = (float)tw / (float)th;

    if( perspective )
	{
        setPerspective();
    }
    else
	{
        setParallel();
    }

    glutPostRedisplay();
}

void Scene::initRender()
{
	// FLAGS
	if ( ambientLighting )
	{
		glEnable(GL_LIGHTING);
	}
	else
	{
		glDisable(GL_LIGHTING);
	}

	// Wireframe
	if ( wireframe )
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}
	else
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
	// Culling
	if ( culling )
	{
		glEnable( GL_CULL_FACE );
		glCullFace( GL_BACK );
	}
	else
	{
		glDisable( GL_CULL_FACE );
	}
	// Z-Buffer
	if ( zbuffer )
	{
		glEnable( GL_DEPTH_TEST );
		glDepthFunc( GL_LESS );
	}
	else
	{
		glDisable( GL_DEPTH_TEST );
	}
	// Sombreado
	if ( smooth_shading )
	{
		glShadeModel( GL_SMOOTH );
	}
	else
	{
		glShadeModel( GL_FLAT );
	}
	// Sentido de las caras
	if( clockwise )
	{
        glFrontFace( GL_CW ); // Horario
    }
    else
	{
        glFrontFace( GL_CCW ); // Antihorario
    }
}

void Scene::renderCamera()
{
	 // Camara
    if ( activeCamera != 0 )
    {
    	// Camara normal
    	if ( !activeCamera->tracing )
		{
			glRotatef(view_rotation.x, 1.0, 0.0, 0.0);
			glRotatef(view_rotation.y, 0.0, 1.0, 0.0);
			glRotatef(view_rotation.z, 0.0, 0.0, 1.0);

			glMultMatrixf(view_rotate);

			glTranslatef(view_position[0], view_position[1], view_position[2]);

			glScalef(scale, scale, scale);
		}
		// Camara de seguimiento
		else
		{
			float angulo = ((objSeleccion->rotation.y)*PI) / 180.0;

			gluLookAt(  objSeleccion->position.x + activeCamera->position.x * sin(angulo),
						objSeleccion->position.y + activeCamera->position.y,
						objSeleccion->position.z + activeCamera->position.z * cos(angulo),
						objSeleccion->position.x + activeCamera->rotation.x,
						objSeleccion->position.y + activeCamera->rotation.y,
						objSeleccion->position.z + activeCamera->rotation.z,
						0.0, 1.0, 0.0 );
		}
    }
}

void Scene::renderSkybox()
{
	// No queremos que le afecte la iluminacion
	glDisable(GL_LIGHTING);
	float x = 0, y = 0, z = 0;
	float width = 1000, height = 1000, length = 1000;

	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;

	glPushMatrix();
	// Lo posicionamos en la altura que mejor se ve el fondo
	glTranslatef(0.0, height/2-300, 0.0);

	// Draw Front side
	if (textures == 1) SkyboxTexture[SKYFRONT]->bind();
	else glColor4f(0.2, 0.4, 0.6, 1.0); // El resto de caras se aplican con este color
	glBegin(GL_QUADS);
		glTexCoord2f(-1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(-1.0f, -1.0f); glVertex3f(x,		  y+height, z+length);
		glTexCoord2f(0.0f, -1.0f); glVertex3f(x+width, y+height, z+length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z+length);
	glEnd();

	// Draw Back side
	if (textures == 1) SkyboxTexture[SKYBACK]->bind();
	glBegin(GL_QUADS);
		glTexCoord2f(-1.0f, 0.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(-1.0f, -1.0f); glVertex3f(x+width, y+height, z);
		glTexCoord2f(0.0f, -1.0f); glVertex3f(x,		  y+height,	z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
	glEnd();

	// Draw Left side
	if (textures == 1) SkyboxTexture[SKYLEFT]->bind();
	glBegin(GL_QUADS);
		glTexCoord2f(-1.0f, -1.0f); glVertex3f(x,		  y+height,	z);
		glTexCoord2f(0.0f, -1.0f); glVertex3f(x,		  y+height,	z+length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(-1.0f, 0.0f); glVertex3f(x,		  y,		z);
	glEnd();

	// Draw Right side
	if (textures == 1) SkyboxTexture[SKYRIGHT]->bind();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(-1.0f, 0.0f); glVertex3f(x+width, y,		z+length);
		glTexCoord2f(-1.0f, -1.0f); glVertex3f(x+width, y+height,	z+length);
		glTexCoord2f(0.0f, -1.0f); glVertex3f(x+width, y+height,	z);
	glEnd();

	// Draw Up side
	if (textures == 1) SkyboxTexture[SKYUP]->bind();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y+height, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y+height, z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z+length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
	glEnd();

	// Draw Down side
	if (textures == 1) SkyboxTexture[SKYDOWN]->bind();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y,		z+length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y,		z);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();

	if ( ambientLighting ) {
		glEnable(GL_LIGHTING);
	}
}


void Scene::renderReflection()
{
	//REFLEJOS
	// Stencil Buffer a ceros para escribir en el
	glColorMask(0,0,0,0);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glDisable(GL_DEPTH_TEST);

	// Renderizamos la carretera para que se escriba en el Stencil Buffer
	road->draw();

	// Cambiamos modo para poder escribir en el Stencil Buffer
	glEnable(GL_DEPTH_TEST);
	glColorMask(1,1,1,1);
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	// Aplicamos plano de recortado definido por la ecuaci�n eqr
	double eqr[] = {0.0f,-1.0f, 0.0f, 0.0f};
	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, eqr);

	// Dibujamos los objetos invertidos en y
	glPushMatrix();
		glFrontFace( GL_CW );
		glScalef(1.0f, -1.0f, 1.0f);
		renderLights();
		//DIBUJO LOS OBJETOS
		renderObjects();
		glFrontFace( GL_CCW );
	glPopMatrix();

	// Deshabilitamos plano de recortado y dibujamos suelo semitransparente
	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_STENCIL_TEST);

	// La parte final de la carretera semitransparente se hace desde fuera de esta funcion
	road->setTransparent(true);
}

void Scene::renderLights()
{
	for ( size_t i = 0; i < lights.size(); i++ )
	{
		if( lights[i]->enabled )
		{
			glEnable(lights[i]->numLight);
			// Calcula el color segun la intensidad
			float intensity = lights[i]->intensity;
			float ambient[4] = { lights[i]->ambient[0]*intensity, lights[i]->ambient[1]*intensity, lights[i]->ambient[2]*intensity, 1.0f };
			glLightfv(lights[i]->numLight, GL_AMBIENT,  ambient);
			float diffuse[4] = { lights[i]->diffuse[0]*intensity, lights[i]->diffuse[1]*intensity, lights[i]->diffuse[2]*intensity, 1.0f };
			glLightfv(lights[i]->numLight, GL_DIFFUSE,  diffuse);
			glLightfv(lights[i]->numLight, GL_POSITION, lights[i]->position);
		}
		else
		{
			glDisable(lights[i]->numLight);
		}
	}
}

void Scene::updateObjects()
{
	for(size_t i = 0; i < objects.size(); i++)
	{
		switch ( objects[i]->id )
		{
			case COCHE:
				// Si el coche es el escogido actualizamos la rotacion
				if ( objects[i] == objSeleccion )
				{
					// Actualizamos la velocidad y rotacion del coche
					if (moving == 1) {
						carSpeed += ACCELERATION;
					}
					else if(moving == -1) {
						carSpeed -= ACCELERATION;
					}

					if (wheelsRotation == 1 && carSpeed != 0.0) {
						objSeleccion->rotation.y += 10.0f * carSpeed;
					}
					else if(wheelsRotation == -1 && carSpeed != 0.0) {
						objSeleccion->rotation.y -= 10.0f * carSpeed;
					}

					// Pero solo se la empezamos a aplicar cuando se mueva
					if ( carSpeed != 0 )
					{
						if ( carSpeed > MAX_SPEED ) carSpeed = MAX_SPEED;
						else if ( carSpeed < -MAX_SPEED ) carSpeed = -MAX_SPEED;

						carSpeed *= FRICTION;
						// Detenemos el coche si la velocidad es muy baja
						if( inRange(carSpeed, 0.002f) ) carSpeed = 0.0f;

						float angulo = (objects[i]->rotation.y * PI) / 180.0;

						float coseno = cos(angulo);
						float seno = sin(angulo);

						objects[i]->position.x += carSpeed * seno;
						objects[i]->position.z += carSpeed * coseno;
					}
				}
				break;
			case RUEDA_D: // Aplicamos la rotacion si pertenecen al seleccionado actual
				if ( objects[i]->parent == objSeleccion )
				{
					// Rotacion en eje y
					if ( wheelsRotation != 0 )
					{
						if ( objects[i]->rotation.y > MAX_WHEEL_ROTATION) {
							objects[i]->rotation.y = MAX_WHEEL_ROTATION;
						}
						if ( objects[i]->rotation.y < -MAX_WHEEL_ROTATION) {
							objects[i]->rotation.y = -MAX_WHEEL_ROTATION;
						}

						objects[i]->rotation.y += (wheelsRotation * WHEEL_ROTATION_Y); // wheelsRotation guarda el signo
					}
					else // Las ruedas vuelven automaticamente a la posicion inicial
					{
						if ( objects[i]->rotation.y > 0.0f ) {
							objects[i]->rotation.y -= WHEEL_ROTATION_Y;
						}
						else if ( objects[i]->rotation.y < 0.0f ) {
							objects[i]->rotation.y += WHEEL_ROTATION_Y;
						}

						// Para evitar posibles problemas de imprecision, si estamos cerca de 0, es 0
						// Rango de -WHEEL_ROTATION_Y a WHEEL_ROTATION_Y pasando por cero
						if ( inRange(objects[i]->rotation.y, WHEEL_ROTATION_Y) )
							objects[i]->rotation.y = 0.0f;
					}

					// Rotacion en eje x
					if ( carSpeed != 0.0f )
					{
						objects[i]->rotation.x += ( WHEEL_ROTATION_X * carSpeed );
					}
				}
				break;
			case RUEDA_T: // Aplicamos la rotacion si pertenecen al seleccionado actual
				if ( objects[i]->parent == objSeleccion )
				{
					// Rotacion en eje x
					if ( carSpeed != 0.0f )
					{
						objects[i]->rotation.x += ( WHEEL_ROTATION_X * carSpeed );
					}
				}
				break;
			case VOLANTE:
				if ( objects[i]->parent == objSeleccion )
				{
					// Rotacion en eje y
					if ( wheelsRotation != 0 )
					{
						if ( objects[i]->rotation.z > MAX_WHEEL_ROTATION*3) {
							objects[i]->rotation.z = MAX_WHEEL_ROTATION*3;
						}
						if ( objects[i]->rotation.z < -MAX_WHEEL_ROTATION*3) {
							objects[i]->rotation.z = -MAX_WHEEL_ROTATION*3;
						}

						objects[i]->rotation.z -= (wheelsRotation * WHEEL_ROTATION_Y*3); // wheelsRotation guarda el signo
					}
					else
					{
						if ( objects[i]->rotation.z > 0.0f ) {
							objects[i]->rotation.z -= WHEEL_ROTATION_Y*3;
						}
						else if ( objects[i]->rotation.z < 0.0f ) {
							objects[i]->rotation.z += WHEEL_ROTATION_Y*3;
						}

						if ( inRange(objects[i]->rotation.z, WHEEL_ROTATION_Y*3) )
							objects[i]->rotation.z = 0.0f;
					}
				}
				break;
		}
	}
}


void Scene::renderObjects()
{
	if(show_skybox)
	{
		renderSkybox();
	}
	for(size_t i = 0; i < objects.size(); i++)
	{
		switch ( objects[i]->id )
		{
			case CARRETERA:
				if ( show_carretera ) objects[i]->draw();
				break;
			case COCHE:
				if ( show_car ) objects[i]->draw();
				break;
			case RUEDA_D:
				if ( show_ruedas ) objects[i]->draw();
				break;
			case RUEDA_T:
				if ( show_ruedas ) objects[i]->draw();
				break;
			case ACERA:
				if ( show_acera ) objects[i]->draw();
				break;
			case ROTONDA:
				if ( show_rotonda ) objects[i]->draw();
				break;
			case FAROLA:
				if ( show_farolas ) objects[i]->draw();
				break;
			case EDIFICIO:
				if ( show_edificios ) objects[i]->draw();
				break;
			case PAPELERA:
				if ( show_papeleras ) objects[i]->draw();
				break;
			case CUBO_BASURA:
				if ( show_cubos ) objects[i]->draw();
				break;
			case BANCO:
				if ( show_bancos ) objects[i]->draw();
				break;
			case SENAL:
				if ( show_senales ) objects[i]->draw();
				break;
			default:
				objects[i]->draw();
				break;
		}
	}
}

void Scene::render()
{
	frame++;
	actualTime = glutGet(GLUT_ELAPSED_TIME);
	if (actualTime - timebase > 1000) {
		char s[32];
		sprintf(s,"CarGL (2013-14) FPS:%4.2f", frame * 1000.0 / (actualTime - timebase));
		glutSetWindowTitle(s);
		timebase = actualTime;
		frame = 0;
	}

	// La actualizamos en cada render por si el escalado de la escena
	// ha cambiado, entonces el far se debe actualizar con el escalado
    if(perspective) {
		setPerspective();
    }
    else {
		setParallel();
    }

	// Actualizamos las posiciones de los objetos antes del render
	// de esta manera el renderObjects no las aplica dos veces debido a los reflejos
	updateObjects();

	// Color del fondo por defecto, parecido al del skybox
    glClearColor(0.4, 0.52, 0.6, 1.0);
    //Para los REFLEJOS limpiamos tambien el stencil buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	initRender();

	renderCamera();

	if (show_carretera)
	{
		if (show_reflections) {
			// Dibujamos la escena invertida en modo de reflejo
			renderReflection();
		}
		else { // Como renderReflection la puede haber puesto a true...
			road->setTransparent(false);
		}
		// Las luces las colocamos aqui para que la carretera tambien se vea afectada
		renderLights();
		// En cualquier caso se dibuja la carretera
		road->draw();
	}
	else { // Parece redundante pero esto solo lo hace cuando no se dibuja carretera
		renderLights();
	}

	renderObjects();

	// Comprueba si ha habido algun error de OpenGL
	GLenum errCode;
	const GLubyte *errString;

	if ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
	   fprintf (stderr, "OpenGL Error: %s\n", errString);
	}

    glutSwapBuffers();
}

// Selecciona un objeto a trav�s del rat�n
void Scene::pick3D(int mouse_x, int mouse_y) {
	// formato de buffer, cada cuatro posiciones almacena:
	//      buffer[i]   = n�mero de objetos
    //      buffer[i+1] = profundidad m�nima
    //      buffer[i+2] = profuncidad m�xima
    //      buffer[i+3] = nombre de la pila

    // Tama�o de la ventana (Viewport) [0] es <x>, [1] es <y>, [2] es <ancho>, [3] es <alto>
    GLint	viewport[4];
    GLuint	buffer[2048];
    GLint   hits;
    int     profundidad;
    char    cad[80];
    int     tx, ty, tw, th;

    seleccion = 0;
    GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
    glViewport( tx, ty, tw, th );

    // Establece el vector <viewport> al tama�o y posici�n relativa a la ventana de visualizaci�n
    glGetIntegerv(GL_VIEWPORT, viewport);
	glSelectBuffer(2048, buffer);  // usa <buffer> como nuestro vector para seleccionar

    (void) glRenderMode(GL_SELECT); // Modo Render en SELECCION

	glInitNames();				  // Inicializa la pila de nombres
    glPushName(0);				  // Apila 0 (al menos una entrada) en la pila

    glMatrixMode(GL_PROJECTION);  // Selecciona el modo proyecci�n
    glPushMatrix();				  // Apila la matriz de proyecci�n
    glLoadIdentity();			  // Resetea la matriz (matriz identidad)

    // Crea una matriz que agranda la peque�a porci�n de pantalla donde se ecuentra el rat�n
    gluPickMatrix((GLdouble) mouse_x, (GLdouble) (viewport[3]+viewport[1]-mouse_y), 1.0f, 1.0f, viewport);

    if (this->perspective) {
		gluPerspective(45, aspectRatio, 0.1, projFar*scale);
    }
    else {
		glOrtho(-8, 8, -5, 5, 0.1, projFar*scale);
    }
	glMatrixMode(GL_MODELVIEW);	   	// Selecciona la matriz de ModelView
	glLoadIdentity();
	renderCamera();
	renderObjects();
    glMatrixMode(GL_PROJECTION);   	// Selecciona la matriz de Proyecci�n
    glPopMatrix();				   	// Recupera la matriz de Proyecci�n
    glMatrixMode(GL_MODELVIEW);	   	// Selecciona la matriz de ModelView
    hits=glRenderMode(GL_RENDER);  	// Cambia a modo RENDERIZAR

    if (hits > 0)				   	// Si hay m�s de un objeto
                                   	// (Hits=n�mero de objetos que se hallan en la posici�n del rat�n)
    {
        seleccion = buffer[3];		// Coge como selecci�n el primer objeto de la lista
        profundidad = buffer[1];    // Calcula su profundidad

        for (int i = 1; i < hits; i++)  // Recorre todos los objetos
        {
			// Si el objeto est� m�s cerca que el seleccionado ahora
            if (buffer[i*4+1] < GLuint(profundidad))
			{
            	seleccion = buffer[i*4+3];	    // Selecciona dicho objeto
                profundidad = buffer[i*4+1];	// Calcula su profundidad
            }
		}
    }

	setSelection(seleccion);

	if ( guiManager != 0 )
	{
		sprintf(cad, "%s [%03d, %03d]", objSeleccion->name.c_str(), mouse_x, mouse_y);
		guiManager->sel_tex->set_text(cad);
	}
}

// Una vez realizado el pick3D se busca el objeto seleccionado
void Scene::setSelection(int seleccion)
{
	// Si se ha seleccionado algo, miramos entre los objetos
    // y cogemos el Object padre de este objeto seleccionado
    if ( seleccion != 0 )
	{
		for ( size_t i = 0; i < objects.size(); i++ )
		{
			if ( objects[i]->selectable )
			{
				int maximo = objects[i]->firstDList + objects[i]->dListCount - 1;
				if ( (seleccion >= objects[i]->firstDList && seleccion <= maximo) )
				{
					objSeleccion = objects[i];
					carRotation = objSeleccion->rotation.y;
					iconSelection->parent = objSeleccion;
					carSpeed = 0.0f;
					continue;
				}
			}
		}
	}
}


unsigned int Scene::getObjectList(std::string fileName)
{
	for ( size_t i = 0; i < objects.size(); i++ )
	{
		if ( fileName.compare(objects[i]->fileName) == 0 )
		{ // Ya existe un objeto igual, devolvemos la display list
			return objects[i]->firstDList;
		}
	}

	return 0;
}

Texture* Scene::getTexture(std::string fileName)
{
	for ( size_t i = 0; i < loadedTextures.size(); i++ )
	{
		if ( fileName.compare(loadedTextures[i]->fileName) == 0 )
		{ // Ya existe un objeto igual, devolvemos la display list
			return loadedTextures[i];
		}
	}

	// No hay encontrado la textura, la creamos
	Texture* newTexture = new Texture(fileName);
	loadedTextures.push_back(newTexture);
	return newTexture;
}

Vector3 Scene::normalize(Vector3 v)
{
	float length = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));

	float x = v.x/length;
    float y = v.y/length;
    float z = v.z/length;

	return Vector3(x, y, z);
}

bool Scene::inRange(float value, float range)
{
	if ( value >= -range && value <= range )
		return true;
	else
		return false;
}
