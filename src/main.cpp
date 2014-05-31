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

    // Inicializa los valores de OpenGL para esta Aplicación
    //escena.InitGL();
    //gui.Init(main_window);

    // Crear la escena, antes de pasarle la funcion al displayFunc
    scene = new Scene();
    // Rellena la escena con los objetos y los coloca en la posicion inicial
    scene->initObjects();
    scene->addCamera( 0, 5, -8, 0, 0, 0, true); // Agrega una camara y es la activa

    // GUI
    guiManager = new GuiManager();

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
