#define _CRT_SECURE_NO_WARNINGS
#include "UI_Letters_NonStatic_Static.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "p2List.h"
#include "j1Render.h"
#include "UI.h"
#include"Color.h"


//-----------------------------------------------UI_Letters_NonStatic-----------------------------------------------
UI_Letters_NonStatic::UI_Letters_NonStatic(UI_Type type, iPoint pos, p2SString* string, SDL_Rect rectwriting, bool movable) : UI(type, pos)
{
	this->string = *string;	
	OrigString = *string;
	UI_Collision.x = pos.x;
	UI_Collision.y = pos.y;
	UI_Collision.w = rectwriting.w;
	UI_Collision.h = rectwriting.h;

	UI_Rect.x = rectwriting.x;
	UI_Rect.y = rectwriting.y;
	UI_Rect.w = rectwriting.w;
	UI_Rect.h = rectwriting.h;
	this->movable = movable;
	UI_WritingIMG = rectwriting;
	cursor_rect.w = 1;
	cursor_rect.h = 15;
}

// Destructor
UI_Letters_NonStatic::~UI_Letters_NonStatic()
{

}

void UI_Letters_NonStatic::Draw(UI* item)
{
	SDL_Rect tex_rect = item->GetRect();

	//blit rectangle
	App->render->Blit(App->gui->GetAtlasNotConst(), item->Getpos().x - App->render->camera.x, item->Getpos().y - App->render->camera.y, &tex_rect);

	//blit text
	int w = 0;
	int h = 0;
	App->font->CalcSize(string.GetString(), w, h);
	int x = 0;
	int y = 0;
	x = item->Getpos().x + 20 - App->render->camera.x;
	y = item->Getpos().y + (tex_rect.h / 2) - (h / 2) - App->render->camera.y;

	SDL_Texture* tex_to_blit = App->font->Print(string.GetString(), { 255, 255, 255, 255 });
	App->render->Blit(tex_to_blit, x, y);
	SDL_DestroyTexture(tex_to_blit);

	if (focused == true)
	{
		cursor_rect.x = x + cursor_position;
		cursor_rect.y = y;
		App->render->DrawQuad(cursor_rect, White(0), White(1), White(2), 255);
	}

	/*
	SDL_Rect string_rect;
	string_rect.x = x;
	string_rect.y = y;
	string_rect.w = w;
	string_rect.h = h;
	App->render->DrawQuad(string_rect, Red(0), Red(1), Red(2), 80);
	*/
}

void UI_Letters_NonStatic::HandleInput()
{
	//that threee keys may need a timer and you don't need to press the key X times, only keep it pressed.
	if (first_time_edit == true) {
		string.Clear();
		first_time_edit = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN)
	{
		int start = this->string.Length() - letters_moved - 1;
		this->string.Cut(start, start + 1);
		MoveCursor(0);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		if (letters_moved <= this->string.Length()) {
			MoveCursor(-1);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		if (letters_moved > 0) {
			MoveCursor(1);
		}
	}

	/*
	//for selection
	if ((App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN) || (App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_DOWN))
	{
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{

	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{

	}
	}
	*/

	if (App->input->letter_printed == false)
	{
		int w = 0;
		int h = 0;
		App->font->CalcSize(string.GetString(), w, h);
		SDL_Rect tex_rect = GetColliderRect();

		if (w <= (tex_rect.w - 50))
		{
			//here write
			int position = string.Length() - letters_moved;
			this->string.Insert(position, App->input->new_string);
		}
		MoveCursor(0);
		App->input->letter_printed = true;
	}

	/*
	SDL_FIRSTEVENT       0
	SDL_QUIT      256
	SDL_APP_TERMINATING    257
	SDL_APP_LOWMEMORY    258
	SDL_APP_WILLENTERBACKGROUND  259
	SDL_APP_DIDENTERBACKGROUND      260
	SDL_APP_WILLENTERFOREGROUND     261
	SDL_APP_DIDENTERFOREGROUND      262
	SDL_WINDOWEVENT                 512
	SDL_SYSWMEVENT                  513
	SDL_KEYDOWN                     768
	SDL_KEYUP                       769
	SDL_TEXTEDITING                 770
	SDL_TEXTINPUT                   771
	SDL_MOUSEMOTION       1024
	SDL_MOUSEBUTTONDOWN            1025
	SDL_MOUSEBUTTONUP              1026
	SDL_MOUSEWHEEL       1027
	SDL_JOYAXISMOTION      1536
	SDL_JOYBALLMOTION      1537
	SDL_JOYHATMOTION      1538
	SDL_JOYBUTTONDOWN              1539
	SDL_JOYBUTTONUP                1540
	SDL_JOYDEVICEADDED             1541
	SDL_JOYDEVICEREMOVED           1542
	SDL_CONTROLLERAXISMOTION       1616
	SDL_CONTROLLERBUTTONDOWN       1617
	SDL_CONTROLLERBUTTONUP         1618
	SDL_CONTROLLERDEVICEADDED      1619
	SDL_CONTROLLERDEVICEREMOVED    1620
	SDL_CONTROLLERDEVICEREMAPPED   1621
	SDL_FINGERDOWN                 1792
	SDL_FINGERUP                   1793
	SDL_FINGERMOTION               1794
	SDL_DOLLARGESTURE              2048
	SDL_DOLLARRECORD               2049
	SDL_MULTIGESTURE               2050
	SDL_CLIPBOARDUPDATE            2304
	SDL_DROPFILE                   4096
	SDL_USEREVENT                 32768
	SDL_LASTEVENT                 65535
	*/
}

//left = -1, right = 1, stay = 0
bool UI_Letters_NonStatic::MoveCursor(int leforrright)
{
	bool ret = false;

	if ((leforrright >= -1) || (leforrright <= 1))
	{
		p2SString string_to_edit(string.GetString());
		const char* calcchar = string_to_edit.ModifyStrToGetStrFromStartToPosition(string_to_edit.Length() - letters_moved + leforrright);
		int w = 0;
		int h = 0;
		p2SString string_to_check1(calcchar);
		p2SString string_to_check2("");
		if (string_to_check1 != string_to_check2)
			App->font->CalcSize(calcchar, w, h);
		cursor_position = w;
		letters_moved = letters_moved - leforrright;
		ret = true;
	}
	return ret;
}


void UI_Letters_NonStatic::SetCollision(iPoint * col)
{
	this->UI_Collision.x = col->x;
	this->UI_Collision.y = col->y;
}

void UI_Letters_NonStatic::ModifyNonStatString(UI* item, UI_Collision_Type state)
{
	iPoint itempoint;

	switch (state) {
	case oncollision:
		break;
	case right_click:

		break;

	case left_click:
		if (movable == true) {
			itempoint = item->Getpos();
			int motx, moty;
			App->input->GetMouseMotion(motx, moty);
			itempoint.x += motx;
			itempoint.y += moty;
			item->SetPosition(itempoint.x, itempoint.y);
			UI_Collision.x = itempoint.x;
			UI_Collision.y = itempoint.y;
		}
		
		focused = true;
		break;
	case leftclickoutcollider:
		focused = false;
		break;
	case notcolliding:
		if (string=="") {
			this->string = OrigString;
			first_time_edit = true;
		}
		break;
	}
	if (focused == true) {
		HandleInput();
	}
}




//-----------------------------------------------UI_Letters_Static-----------------------------------------------
UI_Letters_Static::UI_Letters_Static(UI_Type type, iPoint pos, p2SString* string, bool movable) : UI(type, pos)
{
	//this->string.create(string->GetString());
	this->string = *string;
	originalstr = this->string;
	int w, h = 0;
	App->font->CalcSize(string->GetString(), w, h);
	UI_Collision.x = pos.x;
	UI_Collision.y = pos.y;
	UI_Collision.w = w;
	UI_Collision.h = h;
	this->movable = movable;
	
}

// Destructor
UI_Letters_Static::~UI_Letters_Static()
{

}

void UI_Letters_Static::Draw(UI* item)
{
	SDL_Texture *texture_to_blit = App->font->Print(((UI_Letters_Static*)item)->string.GetString());
	App->render->Blit(texture_to_blit, ((UI_Letters_Static*)item)->Getpos().x-App->render->camera.x, ((UI_Letters_Static*)item)->Getpos().y - App->render->camera.y);
	SDL_DestroyTexture(texture_to_blit);
}

bool UI_Letters_Static::onCollision(UI * element)
{

	return false;
}

void UI_Letters_Static::ModifyStatString(UI* item, UI_Collision_Type state)
{
	iPoint itempoint;
	
	switch (state) {
	case oncollision:
		this->string.Clear();
		this->string.create("On it!");
		break;
	case right_click:
		this->string.Clear();
		this->string.create("RightClick");
		
		break;

	case left_click:
		this->string.Clear();
		this->string.create("LeftClick");
		if (movable == true) {
			itempoint = item->Getpos();
			int motx, moty;
			App->input->GetMouseMotion(motx, moty);
			itempoint.x += motx;
			itempoint.y += moty;

			item->SetPosition(itempoint.x, itempoint.y);
			UI_Collision.x = itempoint.x;
			UI_Collision.y = itempoint.y;
		}
		break;
	case notcolliding:
		this->string.Clear();
		this->string = originalstr;
		break;
	}
}

void UI_Letters_Static::SetCollision(iPoint * col)
{
	this->UI_Collision.x = col->x;
	this->UI_Collision.y = col->y;
}


