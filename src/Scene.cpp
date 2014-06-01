#include "Scene.h"
#include "Object.h"

#include <GL/glu.h>
#include <GL/glui.h>

#include <iostream>


Scene::Scene()
{
    activeCamera = NULL;

    aspectRatio = 1.0;

    initOpenGL();

    timerClock.start();
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
    glEnable(GL_COLOR_MATERIAL);
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
}

Scene::~Scene()
{
    while (!cameras.empty())
    {
        delete cameras.back();
        cameras.pop_back();
    }
    activeCamera = NULL;
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
}

void Scene::initObjects()
{
	// Carretera
	Object* object = new Object("assets/carretera/carretera.obj");
	objects.push_back( object );

	// Aceras
	object = new Object("assets/acera/acera.obj");
	objects.push_back( object );

	// Rotonda, esta la pongo por separado para agregarle la rotacion a la bola
	object = new Object("assets/rotonda/rotonda_base.obj");
	objects.push_back( object );
	object = new Object("assets/rotonda/rotonda_bola.obj", true); // Es transparente
	object->setConstantRotation(0, 1, 0, 0.02);
	objects.push_back( object );

/*
	// Farolas
	object = new Object("assets/farola/farola.obj");
	objects.push_back( object );

	// Bancos
	object = new Object("assets/banco/banco.obj");
	objects.push_back( object );

	// Senales de trafico
	object = new Object("assets/senal_trafico/senal_trafico.obj");
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
    if ( activeCamera != NULL )
    {
        gluLookAt(  activeCamera->position[0], activeCamera->position[1], activeCamera->position[2],
                    activeCamera->lookAt[0], activeCamera->lookAt[1], activeCamera->lookAt[2],
                    0.0, 1.0, 0.0 );
    }

	for(size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->draw();
	}

	// Comprueba si ha habido algun error de OpenGL
	GLenum errCode;
	const GLubyte *errString;

	if ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
	   fprintf (stderr, "OpenGL Error: %s\n", errString);
	}

    glutSwapBuffers();
}
