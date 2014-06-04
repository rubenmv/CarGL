#include "GuiManager.h"

#include "Scene.h"
#include <GL/glui.h>

// IDs para los callbacks de TGui
#define LISTBOX_LIGHTS			100
#define LISTBOX_CAMERAS			100

#define LIGHT0_ENABLED_ID		200
#define LIGHT1_ENABLED_ID		201
#define LIGHT0_INTENSITY_ID		250
#define LIGHT1_INTENSITY_ID		260

#define ENABLE_ID				300
#define DISABLE_ID				301

#define RESET_ID				400

#define SHADING_FLAT			500
#define SHADING_SMOOTH			501
#define PERSPECTIVE				502
#define PARALLEL				503
#define CLOCKWISE				504
#define CCLOCKWISE				505

GuiManager* guiMan;

void controlCallback( int control )
{
    guiMan->ControlCallback(control);
}

GuiManager::GuiManager(Scene* scene)
{
	guiMan = this;
	this->scene = scene;
	scene->guiManager = this;

	listboxCamarasCount = 0;

	enable_panel2 = 1;
}

GuiManager::~GuiManager()
{
	scene = 0;
	listaLuces.clear();
}

/*
void GuiManager::addLightItem(const char* name, bool enabled)
{
	GLUI_Rollout* luz = new GLUI_Rollout( rolloutLuces, name, false );
	listaLuces.push_back( luz );

	new GLUI_Checkbox( luz, "Enabled", &enable_panel2 );
}
*/

void GuiManager::addLightItem(Scene::Light* light)
{
	GLUI_Rollout* luz = new GLUI_Rollout( rolloutLuces, light->name, false );

	//listaLuces.push_back( luz );

	new GLUI_Checkbox( luz, "Enabled", &light->enabled );
}

void GuiManager::addCameraItem(const char* name, bool active)
{
	listboxCamaras->add_item(listboxCamarasCount, name);
	if ( active )
	{
		listboxCamaras->do_selection(listboxCamarasCount);
	}
	listboxCamarasCount++;
}

void __fastcall GuiManager::init(int main_window) {

	guiMan = this;

    /****************************************************/
    /*        Codigo con el interfaz Grafico GLUI       */
    /****************************************************/
    printf( "GLUI version: %3.2f\n", GLUI_Master.get_version() );

    window_id = main_window;

    /*** Crea a ventana lateral ***/
    glui = GLUI_Master.create_glui_subwindow( window_id, GLUI_SUBWINDOW_RIGHT );

    obj_panel = new GLUI_Rollout(glui, "Propiedades", false );

    /***** Control para las propiedades de scene *****/
    new GLUI_Checkbox( obj_panel, "Modo alambrico", &(scene->wireframe), 1, controlCallback );
    new GLUI_Checkbox( obj_panel, "Z Buffer", &scene->zbuffer, 1, controlCallback );
    new GLUI_Checkbox( obj_panel, "Culling", &scene->culling, 1, controlCallback );
    new GLUI_Checkbox( obj_panel, "Texturas", &scene->textures, 1, controlCallback );
    new GLUI_StaticText( obj_panel, "Modelo de sombreado" );
    new GLUI_Button( obj_panel, "Flat", SHADING_FLAT, controlCallback );
    new GLUI_Button( obj_panel, "Smooth", SHADING_SMOOTH, controlCallback );
    new GLUI_StaticText( obj_panel, "Proyeccion" );
    new GLUI_Button( obj_panel, "Perspectiva", PERSPECTIVE, controlCallback );
    new GLUI_Button( obj_panel, "Paralela", PARALLEL, controlCallback );
    new GLUI_StaticText( obj_panel, "Sentido caras exteriores" );
    new GLUI_Button( obj_panel, "Horario", CLOCKWISE, controlCallback );
    new GLUI_Button( obj_panel, "Antihorario", CCLOCKWISE, controlCallback );

    /***  Rollout de Opciones ***/
    GLUI_Rollout *options = new GLUI_Rollout(glui, "Objetos", false );
    new GLUI_Checkbox( options, "Dibujar Coche", &scene->show_car );
    new GLUI_Checkbox( options, "Dibujar Ruedas", &scene->show_ruedas );
    new GLUI_Checkbox( options, "Dibujar Carretera", &scene->show_carretera );
    new GLUI_Checkbox( options, "Dibujar Acera", &scene->show_acera );
    new GLUI_Checkbox( options, "Dibujar Farolas", &scene->show_farolas );
    new GLUI_Checkbox( options, "Dibujar Edificios", &scene->show_edificios );
    new GLUI_Checkbox( options, "Dibujar Cubos de basura", &scene->show_cubos );
    new GLUI_Checkbox( options, "Dibujar Bancos", &scene->show_bancos );
    new GLUI_Checkbox( options, "Dibujar Rotonda", &scene->show_rotonda );
    new GLUI_Checkbox( options, "Dibujar Senales", &scene->show_senales );

	/*** Luces ***/
    rolloutLuces = new GLUI_Rollout( glui, "Luces", false );

    /*** Camaras ***/
	listboxCamaras = new GLUI_Listbox(glui, "Camaras", 0, LISTBOX_CAMERAS, controlCallback );
    // Añade una separación
    new GLUI_StaticText( glui, "" );
    new GLUI_Checkbox( glui, "Bloquear Movimiento", &enable_panel2 );
    // Añade una separación
    new GLUI_StaticText( glui, "" );
    new GLUI_Button( glui, "Resetear Posicion", RESET_ID, controlCallback );

    // Añade una separación
    new GLUI_StaticText( glui, "" );

    /****** A 'quit' button *****/

    new GLUI_Button( glui, "Salir", 0,(GLUI_Update_CB)exit );

    // Añade una separación
    new GLUI_StaticText( glui, "" );

    new GLUI_StaticText( glui, "Autor: Ruben Martinez Vilar. 2013-2014" );

    // Añade una separación
    new GLUI_StaticText( glui, "" );

    // Añade un panel con texto con el valor de la selección
    GLUI_Panel *panel0 = new GLUI_Panel( glui, "Seleccion");
    sel_tex = new GLUI_StaticText( panel0, "0");


    // Crea la subventana inferior
    glui2 = GLUI_Master.create_glui_subwindow( window_id, GLUI_SUBWINDOW_BOTTOM );

    /**** Link windows to GLUI, and register idle callback ******/
    glui->set_main_gfx_window( window_id );
    glui2->set_main_gfx_window( window_id );

	// Rotacion en todas las direcciones
    view_rot = new GLUI_Rotation(glui2, "Rotacion scene", scene->view_rotate );
    //view_rot->set_spin( 0.5 );

    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_xy = new GLUI_Translation(glui2, "Traslacion scene XY", GLUI_TRANSLATION_XY, scene->view_position );
    trans_xy->set_speed( .05 );

    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_x =  new GLUI_Translation(glui2, "Traslacion scene X", GLUI_TRANSLATION_X, scene->view_position );
    trans_x->set_speed( .05 );

    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_y = new GLUI_Translation( glui2, "Traslacion scene Y", GLUI_TRANSLATION_Y, &scene->view_position[1] );
    trans_y->set_speed( .05 );

    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_z = new GLUI_Translation( glui2, "Traslacion scene Z", GLUI_TRANSLATION_Z, &scene->scale );
    trans_z->set_speed( .005 );
}

void __fastcall GuiManager::ControlCallback( int control )
{
	switch (control)
	{
		case LIGHT0_ENABLED_ID:

			break;
		case LIGHT1_ENABLED_ID:

			break;
		case LIGHT0_INTENSITY_ID:
			/*
		float v[] = { escena.light0_diffuse[0],  escena.light0_diffuse[1], escena.light0_diffuse[2],  escena.light0_diffuse[3] };

		v[0] *= light0_intensity;
		v[1] *= light0_intensity;
		v[2] *= light0_intensity;

		glLightfv(GL_LIGHT0, GL_DIFFUSE, v );
		*/
			break;
		case LIGHT1_INTENSITY_ID:
			/*
		float v[] = {
		  escena.light1_diffuse[0],  escena.light1_diffuse[1],
		  escena.light1_diffuse[2],  escena.light1_diffuse[3] };

		v[0] *= light1_intensity;
		v[1] *= light1_intensity;
		v[2] *= light1_intensity;

		glLightfv(GL_LIGHT1, GL_DIFFUSE, v );
		*/
			break;
		case ENABLE_ID:
			glui2->enable();
			break;
		case DISABLE_ID:
			glui2->disable();
			break;
		case RESET_ID:
			/*
		memcpy(escena.view_position, view_position_c, 3*sizeof(float));
		//memcpy(escena.view_rotate,view_rotate_c,16*sizeof(float));
		view_rot->reset();
		escena.scale = 1.0;
		*/
			break;
		case SHADING_FLAT:
			scene->smooth_shading = 0;
			break;
		case SHADING_SMOOTH:
			scene->smooth_shading = 1;
			break;
		case PERSPECTIVE:
			scene->perspective = 1;
			scene->reshape(0,0);
			break;
		case PARALLEL:
			scene->perspective = 0;
			scene->reshape(0,0);
			break;
		case CLOCKWISE:
			scene->clockwise = 1;
			break;
		case CCLOCKWISE:
			scene->clockwise = 0;
			break;
		case LISTBOX_LIGHTS:

			break;
	}
}

void __fastcall GuiManager::Mouse(int button, int button_state, int x, int y )
{
    scene->pick3D(x, y);
}

void __fastcall GuiManager::Idle(void)
{
	/* According to the GLUT specification, the current window is
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
	if ( glutGetWindow() != window_id )
    glutSetWindow(window_id);

  /*  GLUI_Master.sync_live_all();  -- not needed - nothing to sync in this
                                       application  */
    if (enable_panel2)

        glui2->enable();
    else
        glui2->disable();

  glutPostRedisplay();
}

void __fastcall GuiManager::Reshape(int x, int y)
{

}

void __fastcall GuiManager::Motion(int x, int y)
{

}

void __fastcall GuiManager::output(int x, int y, float r, float g, float b, int font, char* string)
{

}

