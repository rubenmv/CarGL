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

    glutPostRedisplay();
}

static void specialKey(int key, int x, int y)
{
    glutPostRedisplay();
}

void mouse(int button, int button_state, int x, int y )
{
    //gui.Mouse(button, button_state, x, y);
}

void idle()
{
   //gui.Idle();
}

void reshape(int x, int y)
{
   scene->reshape(x, y);
}

void motion(int x, int y)
{
    //gui.Motion(x, y);
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
    scene = new Scene();
    // Rellena la escena con los objetos y los coloca en la posicion inicial
    scene->initObjects();
    scene->addCamera( 0.0, 30.0, 30.0, 0.0, 0.0, 0.0, true); // Agrega una camara y es la activa

    // GUI
    guiManager = new GuiManager(scene);
    guiManager->init(main_window);

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
