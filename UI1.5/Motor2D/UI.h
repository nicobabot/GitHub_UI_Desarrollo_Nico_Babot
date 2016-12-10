#ifndef __UI_H__
#define __UI_H__
#include"j1App.h"
#include "p2Point.h"
#include "SDL\include\SDL.h"
#include"p2List.h"
#include"j1Input.h"

class UI_Image;

enum UI_Type {
	ui_image,
	ui_button,
	ui_letters_non_static,
	ui_letters_static,
	ui_slider,
	ui_window,
	//--------
	ui_window_to_window,
	ui_image_to_window,
	ui_button_to_window,
	ui_letters_non_static_to_window,
	ui_letters_static_to_window,
	ui_slider_to_window,

	unknown
};

enum UI_Collision_Type {
	right_click,
	left_click,
	oncollision,
	leftclickoutcollider,
	notcolliding
};

class UI
{
public:

	UI() {}

	UI(UI_Type type, iPoint pos, SDL_Rect* rect=nullptr, bool movable=false) : type(type), pos(pos), movable(movable)
	{
		if (rect != nullptr) {
			UI_Rect = *rect;
		}
	}


	// Destructor
	virtual ~UI() {};

	// Called each loop iteration
	virtual void Update(float dt) {};

	const UI_Type GetType() {
		return type;
	}

	const SDL_Rect GetRect() {
		return UI_Rect;
	}

	const SDL_Rect  GetColliderRect() {
		return UI_Collision;
	}

	virtual void Draw(p2List_item<UI*>* item) {};
	void SetPosition(int x, int y) {
		pos.x = x;
		pos.y = y;
	}
	virtual void HandleInput() {};
	virtual UI_Collision_Type onCollision() {
		UI_Collision_Type ret = notcolliding;
		App->input->GetMousePosition(mousex, mousey);
			if ((mousex > this->UI_Collision.x) && (mousex < this->UI_Collision.x + UI_Collision.w) && (mousey > this->UI_Collision.y) && (mousey < this->UI_Collision.y + UI_Collision.h)) {
				ret = oncollision;
				if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
					ret = left_click;
				}
				if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT) {
					ret = right_click;
				}

			}
			else
			{
				if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
				{
					ret = leftclickoutcollider;
				}
			}
			return ret;
		}
		

	iPoint Getpos();
private:
	int mousex, mousey;

	UI_Type type = UI_Type::unknown;
	iPoint pos;
	
protected:
	SDL_Rect UI_Rect;
	UI_Image *window;
	SDL_Rect UI_Collision;
	bool movable = false;
};

#endif // __UI_H__