#define _CRT_SECURE_NO_WARNINGS

#include "Parallax.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Input.h"
#include "Player.h"

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

	// THIS IS HARD CODED
	ParallaxBackground0();
	ParallaxBackground1();
	ParallaxBackground2();

	return ret;
}

bool Parallax::PostUpdate()
{
	if (enabled)
	{
		for (int i = 0; i < 5; i++) app->render->DrawTexture(bgTexture, x0 + xstart + parallaxw * i, ystart, NULL);

		for (int i = 0; i < 5; i++) app->render->DrawTexture(bgTexture2, x1 + xstart + parallaxw * i, ystart, NULL);

		for (int i = 0; i < 6; i++) app->render->DrawTexture(bgTexture3, x2 + xstart + parallaxw * i, ystart, NULL);
	}

	return true;
}

bool Parallax::CleanUp()
{
	return true;
}

void Parallax::ParallaxBackground0()
{
	x0 = -app->player->position.x * 0.05f;

	x0 = std::floor(x0);
}

void Parallax::ParallaxBackground1()
{
	x1 = -app->player->position.x * 0.15f;

	x1 = std::floor(x1);
}

void Parallax::ParallaxBackground2()
{
	x2 = -app->player->position.x * 0.35f;

	x2 = std::floor(x2);
}