#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "j1Scene.h"
#include"UI_Letters_NonStatic_Static.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	if(App->map->Load("iso_walk.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if(App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	debug_tex = App->tex->Load("maps/path2.png");

	portal_tex = App->tex->Load("maps/meta3.png");

	potal_map_tile_1 = iPoint(6, 17);
	potal_map_tile_2 = iPoint(18, 15);

	// TODO 3: Create the image (rect {485, 829, 328, 103}) and the text "Hello World" as UI elements
	//SDL_Texture* Print(const char* text, SDL_Color color = {255, 255, 255, 255}, _TTF_Font* font = NULL);

	SDL_Rect banner_rect;
	banner_rect.x = 485;
	banner_rect.y = 829;
	banner_rect.w = 328;
	banner_rect.h = 103;
	iPoint posbanner;
	posbanner.x = 400;
	posbanner.y = 80;
	iPoint posstring;
	posstring.x = 200;
	posstring.y = 50;
	p2SString string;
	string.create("Hello world");



	banner = (UI_Image*)App->gui->CreateImage(ui_image, posbanner, &banner_rect, true);
	
	//----
	iPoint windowpos;
	windowpos.x = 100;
	windowpos.y = 100;
	iPoint windowpos2;
	windowpos2.x = 200;
	windowpos2.y = 150;
	SDL_Rect window_rect;
	window_rect.x = 31;
	window_rect.y = 541;
	window_rect.w = 424;
	window_rect.h = 453;
	Window = (UI_Image*)App->gui->CreateUiWindow(ui_window, windowpos, &window_rect, true);
	Window2= (UI_Image*)App->gui->CreateUiWindow(ui_window_to_window, windowpos2, &window_rect, true, Window);
	text = (UI_Letters_Static*)App->gui->CreateStaticLetters(ui_letters_static_to_window, posstring, &string, true, Window);
	SDL_Rect button_rect;
	button_rect.x = 410;
	button_rect.y = 165;
	button_rect.w = 231;
	button_rect.h = 73;
	SDL_Rect button_rect2;
	button_rect2.x = 410 + button_rect.w;
	button_rect2.y = 165;
	button_rect2.w = 231;
	button_rect2.h = 73;
	SDL_Rect button_rect3;
	button_rect3.x = 1;
	button_rect3.y = 110;
	button_rect3.w = 230;
	button_rect3.h = 71;

	iPoint buttonpos;
	buttonpos.x = 100;
	buttonpos.y = 100;
	p2SString string2;
	string2.create("Button");
	button = (UI_Buttons*)App->gui->CreateButton(ui_button_to_window, buttonpos, &button_rect, &string2, &button_rect2, &button_rect3, true, Window);
	button2 = (UI_Buttons*)App->gui->CreateButton(ui_button_to_window, buttonpos, &button_rect, &string2, &button_rect2, &button_rect3, true, Window2);
	iPoint writingrectpos;
	writingrectpos.x = 50;
	writingrectpos.y = 200;
	SDL_Rect window_rec_writting;
	window_rec_writting.x = 483;
	window_rec_writting.y = 632;
	window_rec_writting.w = 357;
	window_rec_writting.h = 68;
	p2SString string3;
	string3.create("Write here");
	WritingText= (UI_Letters_NonStatic*)App->gui->CreateNonStaticLetters(ui_letters_non_static_to_window, writingrectpos, &string3, window_rec_writting, true, Window);
	

	SDL_Rect vertical_scroll_line;
	vertical_scroll_line.x = 973;
	vertical_scroll_line.y = 786;
	vertical_scroll_line.w = 11;
	vertical_scroll_line.h = 158;

	SDL_Rect vertical_scroll_ball_not_pressed;
	vertical_scroll_ball_not_pressed.x = 1004;
	vertical_scroll_ball_not_pressed.y = 430;
	vertical_scroll_ball_not_pressed.w = 15;
	vertical_scroll_ball_not_pressed.h = 27;

	SDL_Rect vertical_scroll_ball_pressed;
	vertical_scroll_ball_pressed.x = 843;
	vertical_scroll_ball_pressed.y = 321;
	vertical_scroll_ball_pressed.w = 15;
	vertical_scroll_ball_pressed.h = 27;

	SDL_Rect vertical_scroll_background;
	vertical_scroll_background.x = 986;
	vertical_scroll_background.y = 874;
	vertical_scroll_background.w = 11;
	vertical_scroll_background.h = 148;

	p2SString strtext;
	strtext.create("Hola paco");
	slider = (UI_Slider*)App->gui->CreateSlider(ui_slider, &vertical_scroll_ball_not_pressed, strtext, iPoint(100, 100), nullptr, nullptr, &vertical_scroll_background, &vertical_scroll_line, false);


	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{

	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if(origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}

	return true;
}

void j1Scene::Modif_Ui() {
	bool movingitem = false;
	p2List_item<UI*>* temp = App->gui->GetListStart();
	for (temp; temp != nullptr; temp = temp->next) {
		switch (temp->data->GetType()) {
		case UI_Type::ui_letters_static:
			((UI_Letters_Static*)temp->data)->ModifyStatString(temp->data, temp->data->onCollision());
			break;
		case UI_Type::ui_button:
			((UI_Buttons*)temp->data)->ModifyButton(temp->data, temp->data->onCollision());
			break;
		case UI_Type::ui_image:
			((UI_Image*)temp->data)->ModifyImage(temp->data, temp->data->onCollision());
			break;
		case UI_Type::ui_letters_non_static:
			((UI_Letters_NonStatic*)temp->data)->ModifyNonStatString(temp->data, temp->data->onCollision());
			break;
		case UI_Type::ui_window:
			
			if (Modif_Ui_WindowElem(temp->data) == false) {
				((UI_Image*)temp->data)->ModifyImage(temp->data, temp->data->onCollision());
				movingitem = false;
			}
			break;

		}

	}
}

bool j1Scene::Modif_Ui_WindowElem(UI* window) {
	movingitem = false;
	p2PQueue_item<UI*>* temp = ((UI_Image*)window)->StartQueue();
	for (temp; temp != nullptr; temp = temp->next) {
		switch (temp->data->GetType()) {
		case ui_window_to_window:
			Modif_Ui_WindowElem(temp->data);
			if (temp->data->onCollision() == left_click) {
				movingitem = true;
			}
		case ui_image_to_window:
			((UI_Image*)temp->data)->ModifyImage(temp->data, temp->data->onCollision());
			if (temp->data->onCollision() == left_click) {
				movingitem = true;
			}
			break;
		case ui_button_to_window:
			((UI_Buttons*)temp->data)->ModifyButton(temp->data, temp->data->onCollision());
			if (temp->data->onCollision() == left_click) {
				movingitem = true;
			}
			break;
		case ui_letters_non_static_to_window:
			((UI_Letters_NonStatic*)temp->data)->ModifyNonStatString(temp->data, temp->data->onCollision());
			if (temp->data->onCollision() == left_click) {
				movingitem = true;
			}
			break;
		case ui_letters_static_to_window:
			((UI_Letters_Static*)temp->data)->ModifyStatString(temp->data, temp->data->onCollision());
			if (temp->data->onCollision() == left_click) {
				movingitem = true;
			}
			break;
		}	
	}
	return movingitem;
}
			
			

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	// Gui ---
	
	Modif_Ui();


	
	// -------
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	/*if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += ceil(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= ceil(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += ceil(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= ceil(200.0f * dt);
		*/
	


	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count(),
					map_coordinates.x, map_coordinates.y);

	//App->win->SetTitle(title.GetString());

	// Debug pathfinding ------------------------------
	//int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x, p.y);

	iPoint pmt = App->map->MapToWorld(potal_map_tile_1.x, potal_map_tile_1.y);
	App->render->Blit(portal_tex, pmt.x, pmt.y - 7);
	pmt = App->map->MapToWorld(potal_map_tile_2.x, potal_map_tile_2.y);
	App->render->Blit(portal_tex, pmt.x, pmt.y - 7);

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for(uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		App->render->Blit(debug_tex, pos.x, pos.y);
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
