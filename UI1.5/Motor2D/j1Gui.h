#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2List.h"
#include "Ui.h"
#include "UI_Image_Buttons_Letters.h"
#include"p2PQueue.h"

#define CURSOR_WIDTH 2
// TODO 1: Create your structure of classes

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions
	// Factory
	//UI* Create(UI_Type entity_type, iPoint pos, SDL_Rect* rect = nullptr, p2SString* string = nullptr, bool movable=false);
	UI* CreateButton (UI_Type entity_type, iPoint pos, SDL_Rect* rect, p2SString* string, SDL_Rect* rect2=nullptr, SDL_Rect* rect3 = nullptr, bool movable = false, UI_Image *window_point = nullptr);
	UI* CreateImage(UI_Type entity_type, iPoint pos, SDL_Rect* rect, bool movable = false, UI_Image *window_point = nullptr);
	UI* CreateStaticLetters(UI_Type entity_type, iPoint pos, p2SString* string, bool movable = false, UI_Image *window_point = nullptr);
	UI* CreateNonStaticLetters(UI_Type entity_type, iPoint pos, p2SString* string, SDL_Rect writingrect ,bool movable = false, UI_Image *window_point=nullptr);
	UI* CreateUiWindow(UI_Type entity_type, iPoint pos, SDL_Rect* rect, bool movable = false, UI_Image *window_point = nullptr);
	UI* CreateSlider(UI_Type type, SDL_Rect* rect, p2SString text, iPoint pos, SDL_Rect * textrect = nullptr, SDL_Rect * ViewPortRect = nullptr, SDL_Rect * VerticalSliderBackgroundRect = nullptr, SDL_Rect * VerticalSliderLineRect = nullptr, bool movable = false);
	bool Delete(UI* entity_to_delete);

	p2List_item<UI*>* GetListStart();

	const SDL_Texture* GetAtlas() const;

	SDL_Texture* GetAtlasNotConst();

	void DebugDrawer(UI* item);

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;

	p2List<UI*> UI_Elements;
	float UI_update = 1.0f / 5.0f;//update 5 times per second
	float accumulated_update_time = 0.0f;
	bool debugcollisions = false;

};

#endif // __j1GUI_H__