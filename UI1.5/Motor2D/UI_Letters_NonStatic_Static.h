#ifndef __UI_LETTERS_NONSTATIC_STATIC_H__
#define __UI_LETTERS_NONSTATIC_STATIC_H__

#include "UI_Image_Buttons_Letters.h"

//-----------------------------------------------UI_Letters_NonStatic-----------------------------------------------
class UI_Letters_NonStatic : public UI
{
public:

	UI_Letters_NonStatic(UI_Type type, iPoint pos, p2SString* string, SDL_Rect rectwriting,bool movable = false);

	// Destructor
	~UI_Letters_NonStatic();

	void Draw(UI* item);
	void ModifyNonStatString(UI* item, UI_Collision_Type state);
	void HandleInput();
	//bool onCollision(UI* element);
	void SetCollision(iPoint *col);
	bool MoveCursor(int leforrright);
private:
	char *text;
	char *composition;
	p2SString string;
	SDL_Rect UI_WritingIMG;
	bool end_recttext = false;
	bool first_time_edit = true;
	bool text_input = false;
	bool focused = false;
	int cursor_position = 0;
	int letters_moved = 0;
	p2SString OrigString;
	SDL_Rect cursor_rect;
};

//-----------------------------------------------UI_Letters_Static-----------------------------------------------
class UI_Letters_Static : public UI
{
public:

	UI_Letters_Static(UI_Type type, iPoint pos, p2SString* string, bool movable=false);

	// Destructor
	~UI_Letters_Static();

	void Draw(UI* item);
	bool onCollision(UI* element);
	void ModifyStatString(UI* item, UI_Collision_Type state);
	void SetCollision(iPoint *col);
private:
	p2SString originalstr;
	p2SString string;

};

#endif // __UI_LETTERS_NONSTATIC_STATIC_H__