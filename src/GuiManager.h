#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include "Scene.h"
#include <vector>
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

	// live variables usadas por GLUI
	GLUI_StaticText *sel_tex;

	int             enable_panel2;

	// ooo punteros a los paneles
	GLUI            *glui, *glui2;
	GLUI_Panel      *obj_panel;
	GLUI_Rotation   *view_rot;

	GLUI_Rollout* rolloutLuces;
	GLUI_Rollout* rolloutCoches;
	GLUI_Listbox *listboxCamaras;
	unsigned int listboxCamarasCount;

	// Para agregar elementos a las listbox/rollouts dinamicamente segun se crean
	void addLightItem(Scene::Light* light);
	void addCarItem(Object* car);
	void addCameraItem(const char* name, bool active);


private:
    int             window_id;

	Scene* scene;
};

extern GuiManager* guiMan;

#endif // GUIMANAGER_H
