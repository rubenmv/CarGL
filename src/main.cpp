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

void keyboard(unsigned char Key, int x, int y)
{
    switch(Key)
    {
        case 27:
        case 'q': //con q se cierra la aplicacion
            exit(0);
            break;
    }
}

static void specialKey(int key, int x, int y)
{
	Object* car = scene->objSeleccion;

    switch (key)
    {
        case GLUT_KEY_UP:   // El coche avanza
        	scene->moving = 1;
            //car->position.x += 0.1;
            scene->rotationSign.z = 1.0;
            break;
        case GLUT_KEY_DOWN:   // El coche retrocede
        	scene->moving = -1;
			//car->position.x -= 0.1;
            scene->rotationSign.z = -1.0;
            break;
		 case GLUT_KEY_LEFT:   // Izquierda
            scene->rotationSign.y = 1.0;
            break;
        case GLUT_KEY_RIGHT:   // Derecha
            scene->rotationSign.y = -1.0;
            break;
    }
}

void mouse(int button, int button_state, int x, int y )
{
    guiManager->Mouse(button, button_state, x, y);
}

void idle()
{
   guiManager->Idle();
}

void reshape(int x, int y)
{
   scene->reshape(x, y);
}

void motion(int x, int y)
{
    guiManager->Motion(x, y);
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
    // Camaras
    scene->addCamera( "Camara", 18.0, -0.4, 6.0, -10.0, 130.0, 0.0, false); // Agrega una camara estatica
    scene->addCamera( "Camara Aerea", 0.0, -20.0, -40.0, 30.0, 0.0, 0.0, true, false); // Agrega una camara estatica
    scene->addCamera( "Camara seguimiento", 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, false, true); // Agrega una camara de seguimiento y es la activa

    // Luces
    GLfloat light0_ambient_c[4]  = {   1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light0_diffuse_c[4]  = {   1.0f, 0.8f, 0.8f, 1.0f };
	GLfloat light0_specular_c[4] = {   1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light0_position_c[4] = { -30.0f, 1.0f, 0.0f, 1.0f };
	float intensity = 0.6f;
	scene->addLight( "Luz 0", GL_LIGHT0, 1, light0_position_c, intensity, light0_ambient_c, light0_diffuse_c, light0_specular_c);

	GLfloat light1_ambient_c[4]  = { 1.0f,   1.0f,  1.0f, 1.0f };
	GLfloat light1_diffuse_c[4]  = { 0.5f,   0.5f,  0.5f, 1.0f };
	GLfloat light1_specular_c[4] = { 1.0f,   1.0f,  1.0f, 1.0f };
	GLfloat light1_position_c[4] = { 30.0f, 10.0f, 10.0f, 1.0f };
	intensity = 0.2f;
    scene->addLight("Luz 1", GL_LIGHT1, 0, light1_position_c, intensity, light1_ambient_c, light1_diffuse_c, light1_specular_c);

    GLfloat light2_ambient_c[4]  = { 1.0f,   1.0f,  1.0f, 1.0f };
	GLfloat light2_diffuse_c[4]  = { 0.6f,   0.6f,  0.55f, 1.0f };
	GLfloat light2_specular_c[4] = { 1.0f,   1.0f,  1.0f, 1.0f };
	GLfloat light2_position_c[4] = { 40.0f, 10.0f, -100.0f, 1.0f };
	intensity = 0.4f;
    scene->addLight("Luz 2", GL_LIGHT2, 0, light2_position_c, intensity, light2_ambient_c, light2_diffuse_c, light2_specular_c);

    // Funciones para el render
    glutDisplayFunc( render );
    GLUI_Master.set_glutReshapeFunc( reshape ); //Redimensionar
    GLUI_Master.set_glutKeyboardFunc( keyboard );
    GLUI_Master.set_glutSpecialFunc( specialKey );
    GLUI_Master.set_glutMouseFunc( mouse );
    glutMotionFunc( motion );

    /**** We register the idle callback with GLUI, *not* with GLUT ****/
    GLUI_Master.set_glutIdleFunc( idle );

   /**** Regular GLUT main loop ****/
    glutMainLoop();

    delete guiManager;
    delete scene;

    std::cout << "Finalizando..." << std::endl;
    return EXIT_SUCCESS;
}
