/****************************************************************************

    CarGL (main.cpp)

    Práctica de Gráficos por Computador (Grado Ingeniería Multimedia)
  -----------------------------------------------------------------------

    CarsGL (C) Juan Antonio Puchol García (puchol@dccia.ua.es)

    Changelog:

    Versión 1.0 (Noviembre 2012)
        - Versión inicial para Code::Blocks 10.05

    Versión 1.1 (Noviembre 2012)
        - Arreglado el bug de selección incorrecta debido al panel inferior de controles

    Versión 1.2 (Octubre 2013 )
        - Adaptación de la librería GLUI a Code::Blocks 12.11

    NOTA:   Para que esta aplicación compile se necesita tener copiados los
            siguientes ficheros:

            Si es un Windows de 64 bits:
            libgui32.a -> C:\Program Files\CodeBlocks\MinGW\lib
            glut32.lib -> C:\Program Files\CodeBlocks\MinGW\lib

            Si es un Windows de 32 bits:
            libgui32.a -> C:\Program Files (x86)\CodeBlocks\MinGW\lib
            glut32.lib -> C:\Program Files (x86)\CodeBlocks\MinGW\lib

            glui.h     -> C:\Program Files (x86)\CodeBlocks\MinGW\include\GL
            glut.h     -> C:\Program Files (x86)\CodeBlocks\MinGW\include\GL

            glut32.dll -> C:\Windows\system

            ACLARACIÓN:
                Hay dos librerías libglui32.a
                - Renombrar libglui32.10.05.a a libglui32.a para usar Code::Blocks 10.05
                - Renombrar libglui32.12.11.a a libglui32.a para usar Code::Blocks 12.11


****************************************************************************/

#include <iostream>
#include <GL/glui.h>

#include "GuiManager.h"
#include "Scene.h"
#include "Object.h"

GuiManager* guiManager;
Scene* scene;
int mouseDown;

void keyboard(unsigned char Key, int x, int y)
{
    switch(Key)
    {
        case 27:
        case 'Q': //con q se cierra la aplicacion
		case 'q': //con q se cierra la aplicacion
            exit(0);
            break;
		// Activacion/desactivacion de propiedades de la escena
		case 'T': // Texturas
		case 't':
			scene->textures = 1-scene->textures;
			break;
		case 'M': // Mipmapping
		case 'm':
			scene->mipmapping = 1-scene->mipmapping;
			break;
		case 'W': // Wireframe
		case 'w':
			scene->wireframe = 1-scene->wireframe;
			break;
		case 'C': // Culling
		case 'c':
			scene->culling = 1-scene->culling;
			break;
		case 'Z': // Z-Buffer
		case 'z':
			scene->zbuffer = 1-scene->zbuffer;
			break;
		case 'L': // Luz ambiente
		case 'l':
			scene->ambientLighting = 1-scene->ambientLighting;
			break;
		case 'R': // Reflejos
		case 'r':
			scene->show_reflections = 1-scene->show_reflections;
			break;
		case 'K': // Reflejos
		case 'k':
			scene->view_position[0]++;
			break;
    }
}

static void specialKey(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_UP:
        	scene->moving = 1; // acelera
            break;
        case GLUT_KEY_DOWN:
        	scene->moving = -1; // acelera hacia atras
            break;
		 case GLUT_KEY_LEFT:   // Izquierda
            scene->wheelsRotation = 1;
            break;
        case GLUT_KEY_RIGHT:   // Derecha
            scene->wheelsRotation = -1;
            break;
    }
}

static void specialKeyUp(int key, int x, int y)
{
	switch (key)
    {	// Si se suelta alguna de las teclas de acelerar, empieza a pararse
    	case GLUT_KEY_UP:
		case GLUT_KEY_DOWN:
        	scene->moving = 0; // deja de acelerar
            break;
		 case GLUT_KEY_LEFT:
            scene->wheelsRotation = 0;
            break;
        case GLUT_KEY_RIGHT:
            scene->wheelsRotation = 0;
            break;
    }
}

void mouse(int button, int button_state, int x, int y )
{
	guiManager->Mouse(button, button_state, x, y);
}

void motion(int x, int y)
{
	guiManager->Motion(x, y);
}

void idle()
{
   guiManager->Idle();
}

void reshape(int x, int y)
{
   scene->reshape(x, y);
}

void render()
{
    scene->render();
    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    // Inicializa GLUT and crea la ventana principal
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE );
    glutInitWindowPosition( 50, 50 );
    glutInitWindowSize( 1000, 600 );

    int main_window = glutCreateWindow( "CarGL (2013-14)" );

    // Crear la escena, antes de pasarle la funcion al displayFunc
    scene = Scene::instance();
    // GUI
    guiManager = new GuiManager(scene);
    guiManager->init(main_window);

    // Rellena la escena con los objetos y los coloca en la posicion inicial
    scene->initObjects();
    // Camaras															  Seguimiento?, activa?
    scene->addCamera( "*Seguimiento exterior", -6.0, 3.0, -6.0, 0.0, 2.0, 0.0, true, false);
    scene->addCamera( "*Volante", -0.2, 0.9, -0.2, 0.0, 0.9, 0.0, true, false);
    scene->addCamera( "Aerea", 0.0, -100.0, 0.0, 90.0, 0.0, 0.0, false, false);
    scene->addCamera( "Vista ciudad", 50.0, -60.0, -30.0, 45.0, 60.0, 0.0, false, false);
    scene->addCamera( "Oblicua", 0.0, -100.0, 0.0, 90.0, 0.0, 0.0, false, false);
    scene->addCamera( "Rotonda", 5.5, -3.0, -6.0, 10.0, 20.0, 0.0, false, false);
    scene->addCamera( "Calle interior", 20.0, -2, 15.0, 5.0, 160.0, 0.0, false, false);
    scene->addCamera( "Calle exterior", 2.0, -3.0, 3.0, 10.0, 10.0, 0.0, false, true);

    // Luces
    GLfloat light0_ambient_c[4]  = {   1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light0_diffuse_c[4]  = {   1.0f, 0.8f, 0.8f, 1.0f };
	GLfloat light0_specular_c[4] = {   1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light0_position_c[4] = { -30.0f, 1.0f, 0.0f, 1.0f };
	float intensity = 0.6f;
	scene->addLight( "Luz 0", GL_LIGHT0, 0, light0_position_c, intensity, light0_ambient_c, light0_diffuse_c, light0_specular_c);

	GLfloat light1_ambient_c[4]  = { 1.0f,   1.0f,  1.0f, 1.0f };
	GLfloat light1_diffuse_c[4]  = { 0.5f,   0.5f,  0.5f, 1.0f };
	GLfloat light1_specular_c[4] = { 1.0f,   1.0f,  1.0f, 1.0f };
	GLfloat light1_position_c[4] = { 30.0f, 10.0f, 10.0f, 1.0f };
	intensity = 0.6f;
    scene->addLight("Luz 1", GL_LIGHT1, 1, light1_position_c, intensity, light1_ambient_c, light1_diffuse_c, light1_specular_c);

    GLfloat light2_ambient_c[4]  = { 1.0f,   1.0f,  1.0f, 1.0f };
	GLfloat light2_diffuse_c[4]  = { 0.6f,   0.6f,  0.55f, 1.0f };
	GLfloat light2_specular_c[4] = { 1.0f,   1.0f,  1.0f, 1.0f };
	GLfloat light2_position_c[4] = { 40.0f, 10.0f, -100.0f, 1.0f };
	intensity = 0.4f;
    scene->addLight("Luz 2", GL_LIGHT2, 0, light2_position_c, intensity, light2_ambient_c, light2_diffuse_c, light2_specular_c);

    GLfloat light3_ambient_c[4]  = { 1.0f,   1.0f,  1.0f, 1.0f };
	GLfloat light3_diffuse_c[4]  = { 0.6f,   0.6f,  0.55f, 1.0f };
	GLfloat light3_specular_c[4] = { 1.0f,   1.0f,  1.0f, 1.0f };
	GLfloat light3_position_c[4] = { 40.0f, 10.0f, -100.0f, 1.0f };
	intensity = 0.35f;
    scene->addLight("Luz 3", GL_LIGHT3, 0, light3_position_c, intensity, light3_ambient_c, light3_diffuse_c, light3_specular_c);

    GLfloat light4_ambient_c[4]  = { 1.0f,   1.0f,  1.0f, 1.0f };
	GLfloat light4_diffuse_c[4]  = { 0.6f,   0.6f,  0.55f, 1.0f };
	GLfloat light4_specular_c[4] = { 1.0f,   1.0f,  1.0f, 1.0f };
	GLfloat light4_position_c[4] = { 40.0f, 10.0f, -100.0f, 1.0f };
	intensity = 0.2f;
    scene->addLight("Luz 4", GL_LIGHT4, 0, light4_position_c, intensity, light4_ambient_c, light4_diffuse_c, light4_specular_c);

    // Funciones para el render
    glutDisplayFunc( render );
    GLUI_Master.set_glutReshapeFunc( reshape ); //Redimensionar
    GLUI_Master.set_glutKeyboardFunc( keyboard );
    GLUI_Master.set_glutSpecialFunc( specialKey );
    GLUI_Master.set_glutMouseFunc( mouse );
    glutMotionFunc( motion );
	glutSpecialUpFunc( specialKeyUp );

    /**** We register the idle callback with GLUI, *not* with GLUT ****/
    GLUI_Master.set_glutIdleFunc( idle );

   /**** Regular GLUT main loop ****/
    glutMainLoop();

    delete guiManager;
    delete scene;

    std::cout << "Finalizando..." << std::endl;
    return EXIT_SUCCESS;
}
