#include"UI_Slider.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "p2Log.h"
#include "UI.h"
#include "UI_Image_Buttons_Letters.h"
#include "UI_Letters_NonStatic_Static.h"
#include"Color.h"
UI_Slider::UI_Slider()
{
}

UI_Slider::UI_Slider(UI_Type type, SDL_Rect* rect, p2SString text, iPoint pos, SDL_Rect * textrect, SDL_Rect * ViewPortRect, SDL_Rect * VerticalSliderBackgroundRect, SDL_Rect * VerticalSliderLineRect, bool movable) : UI(type, pos, rect, movable)
{
	this->text = text;
	if (textrect != nullptr) {
		this->textrect.x = textrect->x;
		this->textrect.y = textrect->y;
		this->textrect.w = textrect->w;
		this->textrect.h = textrect->h;
	}
	if (ViewPortRect != nullptr) {
		this->ViewPortRect.x = ViewPortRect->x;
		this->ViewPortRect.y = ViewPortRect->y;
		this->ViewPortRect.w = ViewPortRect->w;
		this->ViewPortRect.h = ViewPortRect->h;
	}
	this->VerticalSliderLineRect.x = VerticalSliderLineRect->x;
	this->VerticalSliderLineRect.y = VerticalSliderLineRect->y;
	this->VerticalSliderLineRect.w = VerticalSliderLineRect->w;
	this->VerticalSliderLineRect.h = VerticalSliderLineRect->h;

	this->VerticalSliderBackgroundRect.x = VerticalSliderBackgroundRect->x;
	this->VerticalSliderBackgroundRect.y = VerticalSliderBackgroundRect->y;
	this->VerticalSliderBackgroundRect.w = VerticalSliderBackgroundRect->w;
	this->VerticalSliderBackgroundRect.h = VerticalSliderBackgroundRect->h;

	this->UI_Collision.x = rect->x;
	this->UI_Collision.y = rect->y;
	this->UI_Collision.w = rect->w;
	this->UI_Collision.h = rect->h;

	this->movable = movable;
}

UI_Slider::~UI_Slider()
{
}

void UI_Slider::Draw(UI * item)
{
	App->render->Blit(App->gui->GetAtlasNotConst(), item->Getpos().x - App->render->camera.x, item->Getpos().y - App->render->camera.y, &((UI_Slider*)item)->VerticalSliderBackgroundRect);
	App->render->Blit(App->gui->GetAtlasNotConst(), item->Getpos().x - App->render->camera.x, item->Getpos().y - App->render->camera.y, &((UI_Slider*)item)->VerticalSliderLineRect);
	App->render->Blit(App->gui->GetAtlasNotConst(), item->Getpos().x - App->render->camera.x, item->Getpos().y - App->render->camera.y, &((UI_Slider*)item)->UI_Collision);
}
