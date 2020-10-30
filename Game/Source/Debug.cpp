#define _CRT_SECURE_NO_WARNINGS

#include "Debug.h"

#include "App.h"
#include "Input.h"
#include "Map.h"
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

}