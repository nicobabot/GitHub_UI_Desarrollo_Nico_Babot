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
j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());


	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Gui::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		debugcollisions = !debugcollisions;
	}

	p2List_item<UI*>* temp = UI_Elements.start;
	for (temp; temp != nullptr; temp = temp->next) {
		switch (temp->data->GetType()) {
		case ui_image:
			((UI_Image*)temp->data)->Draw(temp->data);
			break;
		case ui_button:
			((UI_Buttons*)temp->data)->Draw(temp->data);
			break;
		case ui_letters_non_static:
			((UI_Letters_NonStatic*)temp->data)->Draw(temp->data);
			break;
		case ui_letters_static:
			((UI_Letters_Static*)temp->data)->Draw(temp->data);
			break;
		case ui_window:
			((UI_Image*)temp->data)->Draw(temp->data);
			break;
		}
		DebugDrawer(temp->data);
	}


	return true;
}



// Called after all Updates
bool j1Gui::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	for (uint i = 0; i < UI_Elements.count(); ++i)
		if (UI_Elements[i]->GetType()== ui_window) {
		//Hace falta hacer deletes
		}
		else {
			delete UI_Elements[i];
		}


	UI_Elements.clear();

	return true;
}

p2List_item<UI*>* j1Gui::GetListStart() {
	return UI_Elements.start;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

SDL_Texture* j1Gui::GetAtlasNotConst()
{
	return atlas;
}

// class Gui ---------------------------------------------------

/*UI* j1Gui::Create(UI_Type type, iPoint pos, SDL_Rect* rect, p2SString* string, bool movable)
{
	UI* ret = nullptr;

	switch (type)
	{
	case UI_Type::ui_image:
		ret = new UI_Image(type, rect, pos);
		break;
	case UI_Type::ui_button:
		ret = new UI_Buttons(type, rect, pos, string, true);
		break;
	case UI_Type::ui_letters_non_static:
		ret = new UI_Letters_NonStatic(type, pos);
		break;
	case UI_Type::ui_letters_static:
		ret = new UI_Letters_Static(type, pos, string);
		break;
	}
	
	if (ret != nullptr)
		UI_Elements.add(ret);

	return ret;
}*/

UI* j1Gui::CreateButton(UI_Type entity_type, iPoint pos, SDL_Rect* rect, p2SString* string, SDL_Rect* rect2, bool movable, UI_Image *window_point) {
	UI* ret = nullptr;
		ret = new UI_Buttons(entity_type, rect, pos, string, rect2, movable);
		if (entity_type == ui_button) {
			UI_Elements.add(ret);
		}
		if (entity_type == ui_button_to_window && window_point != nullptr) {
			((UI_Buttons*)ret)->Setwindow(window_point);
			window_point->PushQueueWindow(ret);
		}

	return ret;
}

UI* j1Gui::CreateImage(UI_Type entity_type, iPoint pos, SDL_Rect* rect, bool movable, UI_Image *window_point) {
	UI* ret = nullptr;
		ret = new UI_Image(entity_type, rect, pos, movable);
		if (entity_type == ui_image) {
			UI_Elements.add(ret);
		}
		
		if (entity_type == ui_image_to_window && window_point != nullptr) {
			window_point->PushQueueWindow(ret);
		}
	return ret;
}

UI* j1Gui::CreateStaticLetters(UI_Type entity_type, iPoint pos, p2SString* string, bool movable, UI_Image *window_point) {
	UI* ret = nullptr;
		ret = new UI_Letters_Static(entity_type, pos, string, movable);
		if (entity_type == ui_letters_static) {
			UI_Elements.add(ret);
		}

		if (entity_type == ui_letters_static_to_window && window_point != nullptr) {
			window_point->PushQueueWindow(ret);
		}
	
	return ret;
}
UI* j1Gui::CreateNonStaticLetters(UI_Type entity_type, iPoint pos, p2SString* string, SDL_Rect writingrect, bool movable, UI_Image *window_point) {
	UI* ret = nullptr;
	ret = new UI_Letters_NonStatic(entity_type, pos, string, writingrect,movable);
	if (entity_type == ui_letters_non_static) {
		UI_Elements.add(ret);
	}
	if (entity_type == ui_letters_non_static_to_window && window_point != nullptr) {
		window_point->PushQueueWindow(ret);
	}

	return ret;
}

UI* j1Gui::CreateUiWindow(UI_Type entity_type, iPoint pos, SDL_Rect* rect, bool movable, UI_Image *window_point) {
	UI* ret = nullptr;
		ret = new UI_Image(entity_type, rect, pos, movable);
	if (entity_type == ui_window) {
		UI_Elements.add(ret);
	}
	else if (entity_type == ui_window_to_window) {
		window_point->PushQueueWindow(ret);
	}
	return ret;
}

bool j1Gui::Delete(UI* entity_to_delete)
{
	bool ret = false;
	int find = UI_Elements.find(entity_to_delete);

	if (find >= 0) {//if find = -1 means error
		p2List_item<UI*>* item;
		item = UI_Elements.start;
		for (int i = 0; i < find; i++) {
			item = item->next;
		}
		ret = UI_Elements.del(item);
	}
	return ret;
}

void j1Gui::DebugDrawer(UI* item) {
	if (debugcollisions == true) {

		SDL_Rect colliderrect = item->GetColliderRect();
		colliderrect.x -= App->render->camera.x;
		colliderrect.y -= App->render->camera.y;

		int alpha = 80;

		switch (item->GetType())
		{
			/*
			Colors:
			Red, Orange, Yellow, Lime, Green, LightMediumSeaGreeen, Cyan, Blue, DarkBlue, Violet, Purple, Magenta, Black, Grey, White
			*/
		case UI_Type::ui_button_to_window:
		case UI_Type::ui_button:
			App->render->DrawQuad(colliderrect, Blue(0), Blue(1), Blue(2), alpha);
			break;
		case UI_Type::ui_image_to_window:
		case UI_Type::ui_image:
			App->render->DrawQuad(colliderrect, Purple(0), Purple(1), Purple(2), alpha);
			break;
		case UI_Type::ui_letters_non_static_to_window:
		case UI_Type::ui_letters_non_static:
			App->render->DrawQuad(colliderrect, Yellow(0), Yellow(1), Yellow(2), alpha);
			break;
		case UI_Type::ui_letters_static_to_window:
		case UI_Type::ui_letters_static:
			App->render->DrawQuad(colliderrect, Lime(0), Lime(1), Lime(2), alpha);
			break;
		case UI_Type::ui_window_to_window:
		case UI_Type::ui_window:
			App->render->DrawQuad(colliderrect, Lime(1), Lime(2), Lime(2), alpha);
			break;
		}
	}
}