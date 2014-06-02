#include "Scene.h"
#include "Object.h"

#include <GL/glu.h>
#include <GL/glui.h>

#include <iostream>


Scene::Scene()
{
    activeCamera = 0;

    aspectRatio = 1.0;

    initOpenGL();

    timerClock.start();

	// FLAGS
    texturesEnabled = 1;
    wireframeEnabled = 0;

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
}

void Scene::initOpenGL()
{
    // Inicializacion de parametros de OpenGL
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

	//glClearDepth(1.f);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    //glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    // Esto activa el blending, que es necesario para renderizar texturas con canal alpha (transparencia)
	//glEnable (GL_BLEND);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
	const GLfloat light_diffuse[]  = { 0.8f, 0.8f, 0.8f, 1.0f };
	const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

	const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
	const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
	const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat high_shininess[] = { 100.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

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
    activeCamera = 0;
}


Object* Scene::getObject(const char* fileName)
{
	/*
	Object* object = 0;
	for (size_t i = 0; i < objects.size(); i++)
	{
		object = objects[i];
		if ( strcmp(object->fileName.c_str(), fileName) == 0 )
		{
			object = new Object(fileName);

		}
	}
	*/

	return 0;
}

void Scene::initObjects()
{
	// CARRETERA
	Object* object = new Object("assets/carretera/carretera.obj", CARRETERA);
	objects.push_back( object );

	// ACERAS
	object = new Object("assets/acera/acera.obj", ACERA);
	objects.push_back( object );

	// ROTONDA, por separado para agregarle la rotacion y transparencia a la bola
	object = new Object("assets/rotonda/rotonda_base.obj", ROTONDA);
	objects.push_back( object );
	object = new Object("assets/rotonda/rotonda_bola.obj", ROTONDA, true); // Es transparente
	object->setConstantRotation(0, 1, 0, 0.02);
	objects.push_back( object );

	// COCHE
	object = new Object("assets/cart/cart_low.obj", COCHE);
	object->rotation[1] = 90; // rotacion y
	object->position[0] = -5; // x
	object->position[2] = 0.5; // z
	objects.push_back( object );

	// FAROLAS
    float despX = 10;
    float despZ = 3;
    float despY = 0;
    int sign = -1;
    float rotation = 0;

    // En el eje x
    for(int j = 1; j < 3; j++)
	{
		for(int i = 1; i < 3; i++)
		{
			object = new Object("assets/farola/farola.obj", FAROLA);
			object->rotation[1] = rotation;		// rotacion y
			object->position[0] = despX * j;	// x
			object->position[1] = despY;		// y
			object->position[2] = despZ * sign; // z
			objects.push_back( object );

			object = new Object("assets/farola/farola.obj", FAROLA);
			object->rotation[1] = rotation;		// rotacion y
			object->position[0] = despX * -j;	// x
			object->position[1] = despY;		// y
			object->position[2] = despZ * sign; // z
			objects.push_back( object );

			sign *= -1;
			rotation = 180;
		}
		rotation = 0;
	}

	// En el eje z
	rotation = 90;
	for(int j = 1; j < 3; j++)
	{
		for(int i = 1; i < 3; i++)
		{
			object = new Object("assets/farola/farola.obj", FAROLA);
			object->rotation[1] = rotation;		// rotacion y
			object->position[0] = despZ * sign;	// x
			object->position[1] = despY;		// y
			object->position[2] = despX * j;	// z
			objects.push_back( object );

			object = new Object("assets/farola/farola.obj", FAROLA);
			object->rotation[1] = rotation;		// rotacion y
			object->position[0] = despZ * sign;	// x
			object->position[1] = despY;		// y
			object->position[2] = despX * -j;	// z
			objects.push_back( object );

			sign *= -1;
			rotation = -90;
		}
		rotation = 90;
	}

	// SENALES DE TRAFICO
	object = new Object("assets/senal_trafico/senal_trafico.obj", SENAL);
	object->rotation[1] = -90;
	object->position[0] = -6;
	//object->position[1] = 2;
	object->position[2] = 2;
	objects.push_back( object );

/*
	// Bancos
	object = new Object("assets/banco/banco.obj");
	objects.push_back( object );


	*/
}

void Scene::addCamera(float px, float py, float pz, float lx, float ly, float lz, bool active /* = false */)
{
    Camera* camera = new Camera();
    camera->position[0] = px;
    camera->position[1] = py;
    camera->position[2] = pz;
    camera->lookAt[0] = lx;
    camera->lookAt[1] = ly;
    camera->lookAt[2] = lz;

    cameras.push_back(camera);

    if (active)
    {
        activeCamera = camera;
    }
}

void Scene::setPerspective()
{
	perspective = true;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(45, aspectRatio, 1.0, 1000.0);
}

void Scene::setParallel()
{
	perspective = false;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-8, 8, -4.5, 4.5, -1, 100);
}

void Scene::reshape(int x, int y)
{
	int tx, ty, tw, th;
    GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
    glViewport( tx, ty, tw, th );

    aspectRatio = (float)tw / (float)th;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

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

void Scene::render()
{
    glClearColor(0.0, 0.7, 0.9, 1.0);
    //Para los REFLEJOS limpiamos tambien el stencil buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // AQUI VA TODO
    // Camara
    if ( activeCamera != 0 )
    {
        gluLookAt(  activeCamera->position[0], activeCamera->position[1], activeCamera->position[2],
                    activeCamera->lookAt[0], activeCamera->lookAt[1], activeCamera->lookAt[2],
                    0.0, 1.0, 0.0 );
    }

	if ( wireframeEnabled == 1 )
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}
	else
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		// Solo usamos texturas si el wireframe esta desactivado
		if ( texturesEnabled == 1 )
		{
			glEnable( GL_TEXTURE_2D );
		}
	}

	for(size_t i = 0; i < objects.size(); i++)
	{
		if ( objects[i]->id == CARRETERA && show_carretera == 1 )
		{
			objects[i]->draw();
		}
		else if ( objects[i]->id == COCHE && show_car == 1 )
		{
			objects[i]->draw();
		}
		else if ( objects[i]->id == ACERA && show_acera == 1 )
		{
			objects[i]->draw();
		}
		else if ( objects[i]->id == ROTONDA && show_rotonda == 1 )
		{
			objects[i]->draw();
		}
		else if ( objects[i]->id == FAROLA && show_farolas == 1 )
		{
			objects[i]->draw();
		}
		else if ( objects[i]->id == EDIFICIO && show_edificios == 1 )
		{
			objects[i]->draw();
		}
		else if ( objects[i]->id == CUBO_BASURA && show_cubos == 1 )
		{
			objects[i]->draw();
		}
		else if ( objects[i]->id == BANCO && show_bancos == 1 )
		{
			objects[i]->draw();
		}
		else if ( objects[i]->id == SENAL && show_senales == 1 )
		{
			objects[i]->draw();
		}
	}

	glDisable( GL_TEXTURE_2D );

	// Comprueba si ha habido algun error de OpenGL
	GLenum errCode;
	const GLubyte *errString;

	if ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
	   fprintf (stderr, "OpenGL Error: %s\n", errString);
	}

    glutSwapBuffers();
}
