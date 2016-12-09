#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "p2Point.h"

struct SDL_Texture;
class GuiImage;
class GuiText;
class UI;
class UI_Image;
class UI_Letters_Static;
class UI_Letters_NonStatic;
class UI_Buttons;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void Modif_Ui();
	bool Modif_Ui_WindowElem(UI* window);
	
private:
	SDL_Texture* debug_tex;
	/*
	GuiImage* banner;
	GuiText* text;
	*/
	UI_Image* banner;
	UI_Letters_Static* text;
	UI_Buttons* button;
	UI_Buttons* button2;
	UI_Image* Window;
	UI_Image* Window2;
	SDL_Texture* portal_tex;
	UI_Letters_NonStatic* WritingText;
public:
	bool movingitem;
	iPoint potal_map_tile_1;
	iPoint potal_map_tile_2;

};

#endif // __j1SCENE_H__