#define _CRT_SECURE_NO_WARNINGS

#include "Debug.h"
#include "Map.h"
#include "App.h"
#include "Input.h"
#include "Collisions.h"
#include "Log.h";

Debug::Debug() : Module()
{
	name.Create("debug");
}


Debug::~Debug()
{

}

bool Debug::Awake()
{
	return true;
}

bool Debug::Start()
{
	return true;
}

bool Debug::PreUpdate()
{
	return true;
}

bool Debug::Update(float dt)
{
	LOG("hello");
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		ToggleColliders();
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		LoadLvl1();
	}

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		LoadLvL2();
	}

	return ret;
}

bool Debug::PostUpdate()
{

	return true;
}

bool Debug::CleanUp()
{
	return true;
}

void Debug::ToggleColliders()
{
	if (app->collisions->showColliders == false)
	{
		app->collisions->showColliders = true;
	}
	else if (app->collisions->showColliders == true) {
		app->collisions->showColliders = false;
	}
}

void Debug::LoadLvl1()
{
	app->map->CleanUp();
	app->map->Load("level1.tmx");
}

void Debug::LoadLvL2()
{
	app->map->CleanUp();
	app->map->Load("level2.tmx");
}