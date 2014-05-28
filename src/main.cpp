#include <iostream>
#include <GL/glui.h>

void Keyboard(unsigned char Key, int x, int y)
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

static void SpecialKey(int key, int x, int y)
{
    glutPostRedisplay();
}

void Mouse(int button, int button_state, int x, int y )
{
    //gui.Mouse(button, button_state, x, y);
}

void Idle()
{
   //gui.Idle();
}

void Reshape(int x, int y){
   //gui.Reshape(x, y);
}

void Motion(int x, int y){
    //gui.Motion(x, y);
}

void Render()
{
    //escena.Render();
}

int main(int argc, char* argv[])
{
    // Inicializa GLUT and crea la ventana principal
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE );
    glutInitWindowPosition( 50, 50 );
    glutInitWindowSize( 1000, 600 );

    int main_window = glutCreateWindow( "CarGL V1.1 (2012)" );

    // Inicializa los valores de OpenGL para esta Aplicación
    //escena.InitGL();
    //gui.Init(main_window);

    //Digo las funcioones q voy a hacer en el render
    glutDisplayFunc( Render );
    GLUI_Master.set_glutReshapeFunc( Reshape ); //Redimensionar
    GLUI_Master.set_glutKeyboardFunc( Keyboard );
    GLUI_Master.set_glutSpecialFunc( SpecialKey );
    GLUI_Master.set_glutMouseFunc( Mouse );
    glutMotionFunc( Motion );

    /**** We register the idle callback with GLUI, *not* with GLUT ****/
    GLUI_Master.set_glutIdleFunc( Idle );


   /**** Regular GLUT main loop ****/
    glutMainLoop();

    std::cout << "Finalizando..." << std::endl;
    return EXIT_SUCCESS;
}
