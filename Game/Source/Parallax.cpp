#define _CRT_SECURE_NO_WARNINGS

#include "Parallax.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Input.h"
#include "Player.h"
#include "Entities.h"

#include "Optick/include/optick.h"

#include <cmath>

Parallax::Parallax() : Module()
{
	name.Create("parallax");
}

Parallax::~Parallax()
{}

bool Parallax::Awake(pugi::xml_node& config)
{
	pugi::xml_node texture = config.child("texture");

	bgTexturePath = texture.attribute("tex1").as_string();
	bgTexturePath2 = texture.attribute("tex2").as_string();
	bgTexturePath3 = texture.attribute("tex3").as_string();

	return true;
}

bool Parallax::Start()
{
	bgTexture = app->tex->Load(bgTexturePath);
	bgTexture2 = app->tex->Load(bgTexturePath2);
	bgTexture3 = app->tex->Load(bgTexturePath3);

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
	OPTICK_EVENT("ParallaxPostUpdate", Optick::Category::Rendering);

	if (enabled)
	{
		for (int i = 0; i < 5; i++)
			app->render->DrawTexture(bgTexture, x0 + xstart + parallaxw * i, ystart, NULL);

		for (int i = 0; i < 5; i++)
			app->render->DrawTexture(bgTexture2, x1 + xstart + parallaxw * i, ystart, NULL);

		for (int i = 0; i < 6; i++)
			app->render->DrawTexture(bgTexture3, x2 + xstart + parallaxw * i, ystart, NULL);
	}

	return true;
}

bool Parallax::CleanUp()
{
	return true;
}

void Parallax::ParallaxBackground0()
{
	x0 = -app->entities->GetPlayer()->position.x * 0.05f;

	x0 = std::floor(x0);
}

void Parallax::ParallaxBackground1()
{
	x1 = -app->entities->GetPlayer()->position.x * 0.15f;

	x1 = std::floor(x1);
}

void Parallax::ParallaxBackground2()
{
	x2 = -app->entities->GetPlayer()->position.x * 0.35f;

	x2 = std::floor(x2);
}