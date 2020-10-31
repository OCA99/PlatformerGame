#define _CRT_SECURE_NO_WARNINGS

#include "Parallax.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Input.h"

#include <iostream>
#include <math.h>

Parallax::Parallax() : Module()
{

}

Parallax::~Parallax()
{

}

bool Parallax::Awake()
{
	return true;
}

bool Parallax::Start()
{
	
	bgTexture = app->tex->Load("Assets/parallax background/background_0.png");
	bgTexture2 = app->tex->Load("Assets/parallax background/background_1.png");
	bgTexture3 = app->tex->Load("Assets/parallax background/background_2.png");

	return true;
}

bool Parallax::PreUpdate()
{
	return true;
}

bool Parallax::Update(float dt)
{
	bool ret = true;

	ParallaxBackground0();
	ParallaxBackground1();
	ParallaxBackground2();

	return ret;
}

bool Parallax::PostUpdate()
{
		
	for (int i = 0; i < 5; i++) app->render->DrawTexture(bgTexture, x0 + xstart + parallaxw * i, ystart, NULL);

	for (int i = 0; i < 5; i++) app->render->DrawTexture(bgTexture2, x1 + xstart + parallaxw * i, ystart, NULL);

	for (int i = 0; i < 6; i++) app->render->DrawTexture(bgTexture3, x2 + xstart + parallaxw * i, ystart, NULL);

	return true;
}

bool Parallax::CleanUp()
{
	return true;
}

void Parallax::ParallaxBackground0()
{
	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		x0 = x0 - 0.2f;
	}
	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if(x0 < 0)	x0 = x0 + 0.2f;
	}

	floor(x0);
}

void Parallax::ParallaxBackground1()
{
	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		x1 = x1 - 0.7f;
	}
	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if (x1 < 0)	x1 = x1 + 0.7f;
	}

	floor(x1);
}

void Parallax::ParallaxBackground2()
{
	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		x2 = x2 - 1.5f;
	}
	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if (x2 < 0)	x2 = x2 + 1.5f;
	}

	floor(x2);
}