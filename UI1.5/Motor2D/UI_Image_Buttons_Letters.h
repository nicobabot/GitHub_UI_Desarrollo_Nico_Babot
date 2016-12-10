#ifndef __UI_IMAGE_BUTTONS_LETTERS_H__
#define __UI_IMAGE_BUTTONS_LETTERS_H__

#include "p2SString.h"
#include"UI.h"
#include"p2PQueue.h"
#include"p2DynArray.h"
class UI_Letters_Static;

enum Button_State {
	button_pressed,
	button_not_pressed,
	buttononcollision
};


//-----------------------------------------------UI_Image-----------------------------------------------
class UI_Image : public UI
{
public:

	UI_Image();

	UI_Image(UI_Type type, SDL_Rect* rect, iPoint pos, bool movable=false);
	
	// Destructor
	~UI_Image();
	void ModifyImage(UI* item, UI_Collision_Type state);
	void Draw(UI* item);
	void PushQueueWindow(UI* item);
	void MoveQueue(UI* item);
	p2PQueue_item<UI*>* StartQueue();
	void SetCollision(iPoint *col);
private:
	p2PQueue<UI*> Items;

};

//-----------------------------------------------UI_Buttons-----------------------------------------------
class UI_Buttons : public UI
{
public:

	UI_Buttons(UI_Type type, SDL_Rect* rect, iPoint pos, p2SString *string, SDL_Rect* rect2=nullptr, SDL_Rect* rect3 = nullptr, bool movable=false);

	// Destructor
	~UI_Buttons();

	void Draw(UI* item);
	void ModifyButton(UI* item, UI_Collision_Type state);
	void Setwindow(UI_Image *windowpoint);
	UI_Image* GetWindow();
	void SetCollision(iPoint *col);
private:
	SDL_Rect ButtonChange;
	SDL_Rect ButtonChange2;
	p2SString name;
	Button_State button_state;


};


#endif // __UI_IMAGE_BUTTONS_LETTERS_H__