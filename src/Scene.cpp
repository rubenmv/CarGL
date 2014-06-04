#include "Scene.h"
#include "Object.h"

#include "GuiManager.h"
#include <iostream>

float view_rotate_c[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float view_position_c[3] = { 0.0, -2.0, -9.0 };

Scene::Scene()
{
	guiManager = 0;

    activeCamera = 0;

    aspectRatio = 1.0;

    initOpenGL();

    timerClock.start();

	// FLAGS
    textures = 1;
    wireframe = 0;
    culling = 1;
    zbuffer = 1;
	smooth_shading = 0;
	perspective = 1;
	clockwise = 0;

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

    memcpy(view_rotate, view_rotate_c, 16*sizeof(float));
    memcpy(view_position, view_position_c, 3*sizeof(float));
    scale = 1.0f;

    seleccion = 0;
}

void Scene::initOpenGL()
{
	// Algunas de estas propiedades se cambiaran en el renderObjects()
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );

	glClearDepth(1.f);

    //glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    // Las luces se agregan desde addLight
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
    activeCamera = 0;

    guiManager = 0;
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
	Object* object = 0;

	// CARRETERA
	object = new Object("assets/carretera/carretera.obj", CARRETERA);
	objects.push_back( object );

	// ACERAS
	object = new Object("assets/acera/acera.obj", ACERA);
	objects.push_back( object );

	// COCHE
	object = new Object( "assets/cart/cart_low.obj", COCHE,
						Vector3(-5.0, 0.0, 0.5), Vector3(0.0, 90.0, 0.0), true ); // Seleccionable
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
			object = new Object("assets/farola/farola.obj", FAROLA,
								Vector3(despX * j, despY, despZ * sign), Vector3(0.0, rotation, 0.0) );
			objects.push_back( object );

			object = new Object("assets/farola/farola.obj", FAROLA,
								Vector3(despX * -j, despY, despZ * sign), Vector3(0.0, rotation, 0.0) );
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
			object = new Object("assets/farola/farola.obj", FAROLA,
								Vector3(despZ * sign, despY, despX * j), Vector3(0.0, rotation, 0.0) );
			objects.push_back( object );

			object = new Object("assets/farola/farola.obj", FAROLA,
								Vector3(despZ * sign, despY, despX * -j), Vector3(0.0, rotation, 0.0) );
			objects.push_back( object );

			sign *= -1;
			rotation = -90;
		}
		rotation = 90;
	}

	// EDIFICIOS

	// Bloque 1
	object = new Object("assets/edificios/edificio1.obj", EDIFICIO,
						Vector3(-20.0, 0.0, -6.0), Vector3(0.0, 0.0, 0.0) );
	objects.push_back( object );
	object = new Object("assets/edificios/edificio1.obj", EDIFICIO,
						Vector3(-15.0, 0.0, -6.0), Vector3(0.0, 0.0, 0.0) );
	objects.push_back( object );
	object = new Object("assets/edificios/edificio2.obj", EDIFICIO,
						Vector3(-10.0, 0.0, -6.0), Vector3(0.0, 0.0, 0.0) );
	objects.push_back( object );
	object = new Object("assets/edificios/edificio2.obj", EDIFICIO,
						Vector3(-6.0, 0.0, -10.0), Vector3(0.0, 90.0, 0.0) );
	objects.push_back( object );
	object = new Object("assets/edificios/edificio2.obj", EDIFICIO,
						Vector3(-6.0, 0.0, -15.0), Vector3(0.0, 90.0, 0.0) );
	objects.push_back( object );

	// SENALES DE TRAFICO
	object = new Object("assets/senal_trafico/senal_trafico.obj", SENAL,
						Vector3(-6.0, 0.0, 2.0), Vector3(0.0, -90.0, 0.0) );
	objects.push_back( object );

	// ROTONDA, por separado para agregarle la rotacion y transparencia a la bola
	object = new Object("assets/rotonda/rotonda_base.obj", ROTONDA);
	objects.push_back( object );
	// Objetos transparentes al final para preservar la profuncdidad					  No seleccionable, transparente
	object = new Object("assets/rotonda/rotonda_bola.obj", ROTONDA, Vector3(), Vector3(), false, true);
	object->setConstantRotation(0.0, 1.0, 0.0, 0.02);
	objects.push_back( object );
}

void Scene::addCamera(const char* name, float px, float py, float pz, float lx, float ly, float lz, bool isStatic /* = true */, bool active /* = false */)
{
    Camera* camera = new Camera();
    camera->name = name;
    camera->position.x = px;
    camera->position.y = py;
    camera->position.z = pz;
    camera->lookAt.x = lx;
    camera->lookAt.y = ly;
    camera->lookAt.z = lz;
    camera->isStatic = isStatic;

    cameras.push_back(camera);

    if (active)
    {
        activeCamera = camera;
    }

    guiManager->addCameraItem(name, active);
}

void Scene::addLight( const char* name, GLenum numLight, int enabled, float position[3], float ambient[4], float diffuse[4], float specular[4] )
{
    Light* light = new Light();

	light->name = name;
    light->numLight = numLight;
    light->enabled = enabled;

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

    if( perspective == 1 )
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

    // Prepara las caracteristicas de la escena
    initRender();

    // Camara
    if ( activeCamera != 0 )
    {
    	// Camara estatica
    	if ( activeCamera->isStatic )
		{
			glTranslatef( view_position[0], view_position[1], view_position[2] );
			glMultMatrixf(view_rotate);
			glScalef(scale, scale, scale);
		}
		// Camara no estatica, sigue a un objeto
		else
		{
			gluLookAt(  activeCamera->position.x, activeCamera->position.y, activeCamera->position.z,
                    activeCamera->lookAt.x, activeCamera->lookAt.y, activeCamera->lookAt.z,
                    0.0, 1.0, 0.0 );
		}
    }

    renderLights();

	// Objetos
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

void Scene::initRender()
{
	// FLAGS
	// Wireframe
	if ( wireframe == 1 )
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}
	else
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		// Solo usamos texturas si el wireframe esta desactivado
		if ( textures == 1 )
		{
			glEnable( GL_TEXTURE_2D );
		}
	}
	// Culling
	if ( culling == 1 )
	{
		glEnable( GL_CULL_FACE );
		glCullFace( GL_BACK );
	}
	else
	{
		glDisable( GL_CULL_FACE );
	}
	// Z-Buffer
	if ( zbuffer == 1 )
	{
		glEnable( GL_DEPTH_TEST );
		glDepthFunc( GL_LESS );
	}
	else
	{
		glDisable( GL_DEPTH_TEST );
	}
	// Sombreado
	if ( smooth_shading == 1 )
	{
		glShadeModel( GL_SMOOTH );
	}
	else
	{
		glShadeModel( GL_FLAT );
	}
	// Sentido de las caras
	if( clockwise == 1 )
	{
        glFrontFace( GL_CW ); // Horario
    }
    else
	{
        glFrontFace( GL_CCW ); // Antihorario
    }
}

void Scene::renderLights()
{
	for ( size_t i = 0; i < lights.size(); i++ )
	{
		if( lights[i]->enabled == 1 )
		{
			glEnable(lights[i]->numLight);
			// Supongo que tambien habra que actualizar le resto
			// de propiedades por si han cambiado??
			glLightfv(lights[i]->numLight, GL_POSITION, lights[i]->position);
		}
		else
		{
			glDisable(lights[i]->numLight);
		}
	}
}

void Scene::renderObjects()
{
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
}


// Selecciona un objeto a través del ratón
void __fastcall Scene::pick3D(int mouse_x, int mouse_y) {
	// formato de buffer, cada cuatro posiciones almacena:
	//      buffer[i]   = número de objetos
    //      buffer[i+1] = profundidad mínima
    //      buffer[i+2] = profuncidad máxima
    //      buffer[i+3] = nombre de la pila

    // Tamaño de la ventana (Viewport) [0] es <x>, [1] es <y>, [2] es <ancho>, [3] es <alto>
    GLint	viewport[4];
    GLuint	buffer[2048];
    GLint   hits;
    int     profundidad;
    char    cad[80];
    int     tx, ty, tw, th;

    seleccion = 0;
    GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
    glViewport( tx, ty, tw, th );

    // Establece el vector <viewport> al tamaño y posición relativa a la ventana de visualización
    glGetIntegerv(GL_VIEWPORT, viewport);
	glSelectBuffer(2048, buffer);  // usa <buffer> como nuestro vector para seleccionar

    (void) glRenderMode(GL_SELECT); // Modo Render en SELECCION

	glInitNames();				  // Inicializa la pila de nombres
    glPushName(0);				  // Apila 0 (al menos una entrada) en la pila
    glMatrixMode(GL_PROJECTION);  // Selecciona el modo proyección
    glPushMatrix();				  // Apila la matriz de proyección
    glLoadIdentity();			  // Resetea la matriz (matriz identidad)
    // Crea una matriz que agranda la pequeña porción de pantalla donde se ecuentra el ratón
    gluPickMatrix((GLdouble) mouse_x, (GLdouble) (viewport[3]+viewport[1]-mouse_y), 1.0f, 1.0f, viewport);

    // Aplica la Matriz de Perspectiva
    //gluPerspective(45.0f, (GLfloat) (viewport[2]-viewport[0])/(GLfloat) (viewport[3]-viewport[1]), 1.0, 1000.0);
    // Para cambiar la perspectiva desde aqui pondriamos un if para seleccionarla
    gluPerspective(45, aspectRatio, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);	   	// Selecciona la matriz de ModelView
    renderObjects();	 			// Renderiza los objetos a seleccionar
    glMatrixMode(GL_PROJECTION);   	// Selecciona la matriz de Proyección
    glPopMatrix();				   	// Recupera la matriz de Proyección
    glMatrixMode(GL_MODELVIEW);	   	// Selecciona la matriz de ModelView
    hits=glRenderMode(GL_RENDER);  	// Cambia a modo RENDERIZAR

    if (hits > 0)				   	// Si hay más de un objeto
                                   	// (Hits=número de objetos que se hallan en la posición del ratón)
    {
        seleccion = buffer[3];		// Coge como selección el primer objeto de la lista
        profundidad = buffer[1];    // Calcula su profundidad

        for (int i = 1; i < hits; i++)  // Recorre todos los objetos
        {
			// Si el objeto está más cerca que el seleccionado ahora
            if (buffer[i*4+1] < GLuint(profundidad))
			{
            	seleccion = buffer[i*4+3];	    // Selecciona dicho objeto
                profundidad = buffer[i*4+1];	// Calcula su profundidad
            }
		}
    }

    if ( guiManager != 0 )
	{
		sprintf(cad, "%03d [%03d, %03d]", seleccion, mouse_x, mouse_y);
		guiManager->sel_tex->set_text(cad);
	}
}
