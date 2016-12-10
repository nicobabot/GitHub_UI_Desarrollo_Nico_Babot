#include "UI_Image_Buttons_Letters.h"
#include "j1Gui.h"
#include "j1App.h"
#include "j1Render.h"
#include"j1Fonts.h"
#include"UI_Letters_NonStatic_Static.h"

//-----------------------------------------------UI_Image-----------------------------------------------
UI_Image::UI_Image()
{

}

UI_Image::UI_Image(UI_Type type, SDL_Rect* rect, iPoint pos, bool movable) : UI(type, pos, rect, movable)
{
	UI_Collision.x = pos.x;
	UI_Collision.y = pos.y;
	UI_Collision.w = rect->w;
	UI_Collision.h = rect->h;
	
	
	//----
}

// Destructor
UI_Image::~UI_Image()
{

}

void UI_Image::ModifyImage(UI* item, UI_Collision_Type state) {
	iPoint itempoint;
	switch (state) {
	case left_click:
		itempoint = item->Getpos();
		if (movable == true) {
			int motx, moty;
			App->input->GetMouseMotion(motx, moty);
			itempoint.x += motx;
			itempoint.y += moty;
		}
		item->SetPosition(itempoint.x, itempoint.y);
		UI_Collision.x = itempoint.x;
		UI_Collision.y = itempoint.y;
		if (item->GetType() == ui_window || item->GetType() == ui_window_to_window) {
			((UI_Image*)item)->MoveQueue(item);
		}
		break;

	}
}

void UI_Image::Draw(UI* item)
{
	if (item->GetType() == ui_window || item->GetType() == ui_image || item->GetType() == ui_window_to_window) {
		App->render->Blit(App->gui->GetAtlasNotConst(), ((UI_Image*)item)->Getpos().x - App->render->camera.x, ((UI_Image*)item)->Getpos().y - App->render->camera.y, &((UI_Image*)item)->GetRect());
		if (item->GetType() == ui_window || item->GetType() == ui_window_to_window) {
			p2PQueue_item<UI*>* temp = Items.start;
			for (temp; temp != nullptr; temp = temp->next) {
				switch (temp->data->GetType()) {
				case ui_image_to_window:
					((UI_Image*)temp->data)->Draw(temp->data);
					break;
				case ui_button_to_window:
					((UI_Buttons*)temp->data)->Draw(temp->data);
					break;
				case ui_letters_non_static_to_window:
					((UI_Letters_NonStatic*)temp->data)->Draw(temp->data);
					break;
				case ui_letters_static_to_window:
					((UI_Letters_Static*)temp->data)->Draw(temp->data);
					break;
					case ui_window_to_window:
						((UI_Image*)temp->data)->Draw(temp->data);
						break;
				}
				App->gui->DebugDrawer(temp->data);
			}
		}
	}
		
	}

void UI_Image::PushQueueWindow(UI * item)
{
	if (item != nullptr) {
		Items.Push(item, item->GetType());
	}
}

void UI_Image::MoveQueue(UI* item)
{
	iPoint position;
	int motx, moty;
	p2PQueue_item<UI*>* temporal = ((UI_Image*)item)->StartQueue();
	for (temporal; temporal != nullptr; temporal = temporal->next) {
			position=(temporal->data)->Getpos();
			App->input->GetMouseMotion(motx, moty);
			position.x += motx;
			position.y += moty;
			temporal->data->SetPosition(position.x, position.y);
			switch (temporal->data->GetType()) {
			case ui_image_to_window:
				((UI_Image*)temporal->data)->SetCollision(&position);
				break;
			case ui_button_to_window:
				((UI_Buttons*)temporal->data)->SetCollision(&position);
				break;
			case ui_letters_non_static_to_window:
				((UI_Letters_NonStatic*)temporal->data)->SetCollision(&position);
				break;
			case ui_letters_static_to_window:
				((UI_Letters_Static*)temporal->data)->SetCollision(&position);
				break;
			case ui_window_to_window:
				((UI_Image*)temporal->data)->SetCollision(&position);
				((UI_Image*)temporal->data)->MoveQueue(temporal->data);
				break;
			}
			
		App->gui->DebugDrawer(temporal->data);
	}
}

p2PQueue_item<UI*>* UI_Image::StartQueue()
{
	return Items.start;
}

void UI_Image::SetCollision(iPoint * col)
{
	this->UI_Collision.x = col->x;
	this->UI_Collision.y = col->y;
}



//-----------------------------------------------UI_Buttons-----------------------------------------------
UI_Buttons::UI_Buttons(UI_Type type, SDL_Rect* rect, iPoint pos, p2SString *str, SDL_Rect* rect2, SDL_Rect* rect3, bool movable) : UI(type, pos, rect, movable)
{
	this->name = *str;
	UI_Collision.x = pos.x;
	UI_Collision.y = pos.y;
	UI_Collision.w = rect->w;
	UI_Collision.h = rect->h;

	ButtonChange.x = rect2->x;
	ButtonChange.y = rect2->y;
	ButtonChange.w = rect2->w;
	ButtonChange.h = rect2->h;
	
	ButtonChange2.x = rect3->x;
	ButtonChange2.y = rect3->y;
	ButtonChange2.w = rect3->w;
	ButtonChange2.h = rect3->h;
}

// Destructor
UI_Buttons::~UI_Buttons() 
{

}

void UI_Buttons::Draw(UI* item)
{
	SDL_Rect rect = item->GetRect();

	if (this->button_state==Button_State::button_pressed) {
		App->render->Blit(App->gui->GetAtlasNotConst(), item->Getpos().x - App->render->camera.x, item->Getpos().y - App->render->camera.y, &rect);		
	}
	else if (this->button_state == Button_State::buttononcollision) {
		App->render->Blit(App->gui->GetAtlasNotConst(), item->Getpos().x - App->render->camera.x, item->Getpos().y - App->render->camera.y, &((UI_Buttons*)item)->ButtonChange2);
	}
	else {
		App->render->Blit(App->gui->GetAtlasNotConst(), item->Getpos().x - App->render->camera.x, item->Getpos().y - App->render->camera.y, &((UI_Buttons*)item)->ButtonChange);
	}
	int w, h;
	App->font->CalcSize(((UI_Buttons*)item)->name.GetString(), w, h);
	SDL_Texture *texture_to_blit = App->font->Print(((UI_Buttons*)item)->name.GetString());
	App->render->Blit(texture_to_blit, ((UI_Buttons*)item)->Getpos().x + ((item->GetRect().w)/2)-(w/2) - App->render->camera.x, ((UI_Buttons*)item)->Getpos().y + ((item->GetRect().h) / 2) - (h / 2) - App->render->camera.y);
	SDL_DestroyTexture(texture_to_blit);
}

void UI_Buttons::ModifyButton(UI* item,UI_Collision_Type state) {

	iPoint itempoint;
	switch (state) {
	case left_click:
		itempoint = item->Getpos();
		int motx, moty;
		App->input->GetMouseMotion(motx, moty);
		itempoint.x += motx;
		itempoint.y += moty;
	
		item->SetPosition(itempoint.x, itempoint.y);
		UI_Collision.x = itempoint.x;
		UI_Collision.y = itempoint.y;

		this->button_state = button_pressed;
		break;
	case oncollision:
		this->button_state = buttononcollision;
		break;
	case notcolliding:
		this->button_state = button_not_pressed;
		break;
	}

}

void UI_Buttons::Setwindow(UI_Image * windowpoint)
{
	window = windowpoint;
}

UI_Image * UI_Buttons::GetWindow()
{
	return window;
}

void UI_Buttons::SetCollision(iPoint *col)
{
	this->UI_Collision.x = col->x;
	this->UI_Collision.y = col->y;
}


