#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <GL/glui.h>

class Scene;

class GuiManager
{
public:
    GuiManager(Scene* scene);
    virtual ~GuiManager();

	void __fastcall init( int main_window );
	void __fastcall ControlCallback( int control );
	void __fastcall Idle( void ); // ooo BUCLE INFINITO -> GESTOR DE EVENTOS // ooo no deja de pintar ; borra y pinta, borra y pinta. Si digo q el coche cambia un milimetro en la pantalla veo donde se mueve --- CONTROLO Q PULSO Y ADEMAS PINTO
	void __fastcall Reshape( int x, int y  );
	void __fastcall Motion( int x, int y  );
	void __fastcall Mouse(int button, int button_state, int x, int y );

	void __fastcall output(int x, int y, float r, float g, float b, int font, char *string); //Mostrar algo por pantalla


private:
    int             window_id;

	// live variables usadas por GLUI
	int             enable_panel2;
	int             light0_enabled;
	int             light1_enabled;
	int             light2_enabled;
	float           light0_intensity;
	float           light1_intensity;
	float           light2_intensity;

	// ooo punteros a los paneles
	GLUI            *glui, *glui2;
	GLUI_Spinner    *light0_spinner;
	GLUI_Spinner    *light1_spinner;
	GLUI_Spinner    *light2_spinner;
	GLUI_RadioGroup *radio;
	GLUI_Panel      *obj_panel;
	GLUI_Rotation   *view_rot;
	GLUI_StaticText *sel_tex;


	//Si no lo declaro aqui, luego no puedo acceder desde otra funcion
	GLUI_Listbox *ListBoxCamaras;
	GLUI_Listbox *ListBoxCamaras2;


	//Colores coches
	float coche1R;
	float coche1G;
	float coche1B;
	float coche2R;
	float coche2G;
	float coche2B;

	Scene* scene;
};

extern GuiManager* guiMan;

#endif // GUIMANAGER_H
