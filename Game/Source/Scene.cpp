#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "Audio.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{

	app->audio->PlayMusic("Assets/audio/music/song.ogg");

	//screenTexture = app->tex->Load("Assets/title screen/title and end screen.png");
	screenTexture = app->tex->Load("Assets/title screen/reduced title screen.png");
	if (screenTexture == nullptr)LOG("could'nt load title screen");

	titleScreenAnim.PushBack({ 0,0,640,480});
	titleScreenAnim.PushBack({ 0,480,640,480 });

	/*titleScreenAnim.PushBack({ 0,0,206,78 });
	titleScreenAnim.PushBack({ 0,78,206,78 });*/

	gameOverAnim.PushBack({ 0,960,640,480 });
	gameOverAnim.PushBack({ 0,1040,640,480 });

	turnOffAnim.PushBack({ 0,0,0,0, });


	titleScreenAnim.loop = gameOverAnim.loop = true;
	titleScreenAnim.speed = gameOverAnim.speed = 0.5f;

	

	currentLevel.Create("level1.tmx");
	app->map->Load("level1.tmx");

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	switch (gameplayState)
	{
	case(TITLE_SCREEN):
		screenDisplayAnim = &titleScreenAnim;
		break;
	case(PLAYING):
		screenDisplayAnim = &turnOffAnim;
		break;
	case(GAME_OVER_SCREEN):
		screenDisplayAnim = &gameOverAnim;
		break;
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && gameplayState == TITLE_SCREEN)
		gameplayState = PLAYING;

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		app->RequestLoad();
		LOG("LOAD REQUESTED");
	}

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN){
		app->RequestSave();
		LOG("SAVE REQUESTED");
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		LoadLevel("level1.tmx");
	}

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		LoadLevel("level2.tmx");
	}

	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		LoadLevel(currentLevel);
	}

	// 8 to volume down and 9 to volume up
	if (app->input->GetKey(SDL_SCANCODE_8) == KEY_REPEAT) {
		app->audio->VolumeDown();
		LOG("Volume down");
	}if (app->input->GetKey(SDL_SCANCODE_9) == KEY_REPEAT) {
		app->audio->VolumeUp();
		LOG("Volume up");
	}

	return true;
}

void Scene::LoadLevel(SString name)
{
	
	app->player->isDead = false;
	currentLevel = name;
	app->map->Load(name.GetString());
	app->player->Reload();
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	SDL_Rect rect = screenDisplayAnim->GetCurrentFrame();
	
	app->render->DrawTexture(screenTexture, 0, 800, &rect);
	
	

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
