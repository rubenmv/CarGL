#include "GuiManager.h"

#include "Scene.h"
#include <GL/glui.h>

// IDs para los callbacks de TGui
#define LIGHT0_ENABLED_ID    200
#define LIGHT1_ENABLED_ID    201
#define LIGHT0_INTENSITY_ID  250
#define LIGHT1_INTENSITY_ID  260

#define ENABLE_ID            300
#define DISABLE_ID           301

#define RESET_ID             400

GuiManager* guiMan;

void controlCallback( int control )
{
    guiMan->ControlCallback(control);
}

GuiManager::GuiManager(Scene* scene)
{
	guiMan = this;
	this->scene = scene;

/*
	seleccion = 0;
    num_objects = 0;
    num_cars = 0;
*/
    //scale = 1.0;
    //xy_aspect = 1;
    //last_x = 0; // Estos dos parece que no se usan
    //last_y = 0;

/*
    memcpy(view_position, view_position_c, 3*sizeof(float));
    memcpy(view_rotate, view_rotate_c, 16*sizeof(float));

    memcpy(light0_ambient, light0_ambient_c, 4*sizeof(float));

    memcpy(light0_ambient, light0_ambient_c, 4*sizeof(float));
    memcpy(light0_diffuse, light0_diffuse_c, 4*sizeof(float));
    memcpy(light0_specular, light0_specular_c, 4*sizeof(float));
    memcpy(light0_position, light0_position_c, 4*sizeof(float));

    memcpy(light1_ambient, light1_ambient_c, 4*sizeof(float));
    memcpy(light1_diffuse, light1_diffuse_c, 4*sizeof(float));
    memcpy(light1_specular, light1_specular_c, 4*sizeof(float));
    memcpy(light1_position, light1_position_c, 4*sizeof(float));

    memcpy(mat_ambient, mat_ambient_c, 4*sizeof(float));
    memcpy(mat_diffuse, mat_diffuse_c, 4*sizeof(float));
    memcpy(mat_specular, mat_specular_c, 4*sizeof(float));
    memcpy(high_shininess, high_shininess_c, 1*sizeof(float));
    */
}

GuiManager::~GuiManager()
{
	scene = 0;
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

    obj_panel = new GLUI_Rollout(glui, "Propiedades", true );

    /***** Control para las propiedades de scene *****/
    new GLUI_Checkbox( obj_panel, "Modo alambrico", &(scene->wireframeEnabled), 1, controlCallback );
    //new GLUI_Checkbox( obj_panel, "Z Buffer", &scene->z_buffer, 1, controlCallback );
    //new GLUI_Checkbox( obj_panel, "Culling", &scene->culling, 1, controlCallback );
    new GLUI_Checkbox( obj_panel, "Texturas", &scene->texturesEnabled, 1, controlCallback );

    /******** Añade controles para las luces ********/
/*
    // Añade una separación
    new GLUI_StaticText( glui, "" );

    GLUI_Rollout *roll_lights = new GLUI_Rollout(glui, "Luces", false );

    GLUI_Panel *light0 = new GLUI_Panel( roll_lights, "Luz 1" );
    GLUI_Panel *light1 = new GLUI_Panel( roll_lights, "Luz 2" );

    new GLUI_Checkbox( light0, "Encendida", &light0_enabled, LIGHT0_ENABLED_ID, controlCallback );
    light0_spinner = new GLUI_Spinner( light0, "Intensidad:", &light0_intensity,
                            LIGHT0_INTENSITY_ID, controlCallback );
    light0_spinner->set_float_limits( 0.0, 1.0 );
    GLUI_Scrollbar *sb;
    sb = new GLUI_Scrollbar( light0, "Rojo",GLUI_SCROLL_HORIZONTAL,
                            &scene->light0_diffuse[0],LIGHT0_INTENSITY_ID,controlCallback);
    sb->set_float_limits(0,1);
    sb = new GLUI_Scrollbar( light0, "Verde",GLUI_SCROLL_HORIZONTAL,
                            &scene->light0_diffuse[1],LIGHT0_INTENSITY_ID,controlCallback);
    sb->set_float_limits(0,1);
    sb = new GLUI_Scrollbar( light0, "Azul",GLUI_SCROLL_HORIZONTAL,
                            &scene->light0_diffuse[2],LIGHT0_INTENSITY_ID,controlCallback);
    sb->set_float_limits(0,1);

    new GLUI_Checkbox( light1, "Encendida", &light1_enabled, LIGHT1_ENABLED_ID, controlCallback );
    light1_spinner = new GLUI_Spinner( light1, "Intensidad:", &light1_intensity,
                            LIGHT1_INTENSITY_ID, controlCallback );
    light1_spinner->set_float_limits( 0.0, 1.0 );
    sb = new GLUI_Scrollbar( light1, "Rojo",GLUI_SCROLL_HORIZONTAL,
                            &scene->light1_diffuse[0],LIGHT1_INTENSITY_ID,controlCallback);
    sb->set_float_limits(0,1);
    sb = new GLUI_Scrollbar( light1, "Verde",GLUI_SCROLL_HORIZONTAL,
                            &scene->light1_diffuse[1],LIGHT1_INTENSITY_ID,controlCallback);
    sb->set_float_limits(0,1);
    sb = new GLUI_Scrollbar( light1, "Azul",GLUI_SCROLL_HORIZONTAL,
                            &scene->light1_diffuse[2],LIGHT1_INTENSITY_ID,controlCallback);
    sb->set_float_limits(0,1);

    // Añade una separación
    new GLUI_StaticText( glui, "" );
*/

    /***  Rollout de Opciones ***/
    GLUI_Rollout *options = new GLUI_Rollout(glui, "Objetos", true );
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


    /*** Disable/Enable botones ***/

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

	/*
    view_rot = new GLUI_Rotation(glui2, "Rotacion scene", scene->view_rotate );
    view_rot->set_spin( 0.5 );
    new GLUI_Column( glui2, false );
    view_rot = new GLUI_Rotation(glui2, "Rotacion scene Y", scene->view_rotate );
    view_rot->set_spin( 1.0 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_xy = new GLUI_Translation(glui2, "Traslacion scene XY", GLUI_TRANSLATION_XY, scene->view_position );
    trans_xy->set_speed( .005 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_x =  new GLUI_Translation(glui2, "Traslacion scene X", GLUI_TRANSLATION_X, scene->view_position );
    trans_x->set_speed( .005 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_y = new GLUI_Translation( glui2, "Traslacion scene Y", GLUI_TRANSLATION_Y, &scene->view_position[1] );
    trans_y->set_speed( .005 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_z = new GLUI_Translation( glui2, "Traslacion scene Z", GLUI_TRANSLATION_Z, &scene->scale );
    trans_z->set_speed( .005 );
    */

}

void __fastcall GuiManager::ControlCallback( int control )
{
	if ( control == LIGHT0_ENABLED_ID )
	{
		if ( light0_enabled )
		{
		  glEnable( GL_LIGHT0 );
		  light0_spinner->enable();
		}
		else
		{
		  glDisable( GL_LIGHT0 );
		  light0_spinner->disable();
		}
	}
	else if ( control == LIGHT1_ENABLED_ID )
	{
		if ( light1_enabled )
		{
		  glEnable( GL_LIGHT1 );
		  light1_spinner->enable();
		}
		else
		{
		  glDisable( GL_LIGHT1 );
		  light1_spinner->disable();
		}
	}
	else if ( control == LIGHT0_INTENSITY_ID )
	{
		/*
		float v[] = { escena.light0_diffuse[0],  escena.light0_diffuse[1], escena.light0_diffuse[2],  escena.light0_diffuse[3] };

		v[0] *= light0_intensity;
		v[1] *= light0_intensity;
		v[2] *= light0_intensity;

		glLightfv(GL_LIGHT0, GL_DIFFUSE, v );
		*/
	}
	else if ( control == LIGHT1_INTENSITY_ID )
	{
		/*
		float v[] = {
		  escena.light1_diffuse[0],  escena.light1_diffuse[1],
		  escena.light1_diffuse[2],  escena.light1_diffuse[3] };

		v[0] *= light1_intensity;
		v[1] *= light1_intensity;
		v[2] *= light1_intensity;

		glLightfv(GL_LIGHT1, GL_DIFFUSE, v );
		*/
	}
	else if ( control == ENABLE_ID )
	{
		glui2->enable();
	}
	else if ( control == DISABLE_ID )
	{
		glui2->disable();
	}
	else if ( control == RESET_ID)
	{
		/*
		memcpy(escena.view_position, view_position_c, 3*sizeof(float));
		//memcpy(escena.view_rotate,view_rotate_c,16*sizeof(float));
		view_rot->reset();
		escena.scale = 1.0;
		*/
	}
}
