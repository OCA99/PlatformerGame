#include "Entities.h"

#include "Log.h"
#include "App.h"
#include "Textures.h"

#include "Bat.h"
#include "Fruit.h"
#include "Heart.h"
#include "Pig.h"
#include "Knife.h"

#include "Player.h"

#include <stdlib.h>
#include <time.h>

Entities::Entities() : Module()
{
	name.Create("entities");
}

bool Entities::Awake(pugi::xml_node& config)
{
	pugi::xml_node entities = config.child("entities");

	enemySpeed = entities.attribute("enemySpeed").as_int();
	pigHealth = entities.attribute("pigHealth").as_int();
	enemyGravity = entities.attribute("enemyGravity").as_int();
	enemyJumpForce = entities.attribute("enemyJumpForce").as_int();
	knifeSpeed = entities.attribute("knifeSpeed").as_int();

	return true;
}

bool Entities::Load(pugi::xml_node& savedGame)
{

	return true;
}

bool Entities::Save(pugi::xml_node& savedGame)
{

	return true;
}

bool Entities::Start()
{
	batTexture = app->tex->Load("Assets/enemies/bat/bat.png");
	pigTexture = app->tex->Load("Assets/enemies/pig/pig.png");
	fruitTexture = app->tex->Load("Assets/Items/item_spritesheet.png");
	heartTexture = app->tex->Load("Assets/Items/heart_anim.png");
	knifetexture = app->tex->Load("Assets/textures/Main Characters/Virtual Guy/knife.png");

	srand(time(NULL));

	return true;
}

bool Entities::PreUpdate()
{
	return true;
}

bool Entities::Update(float dt)
{
	ListItem<Entity*>* start = entityList.start;

	while (start != nullptr)
	{
		if (start->data->pendingToDelete)
		{
			delete start->data;
			entityList.del(start);
			start = start->next;
			continue;
		}

		start->data->Update(dt);
		start = start->next;
	}

	return true;
}

bool Entities::PostUpdate()
{
	for (int i = 0; i < entityList.count(); i++)
	{
		ListItem<Entity*>* e = entityList.At(i);
		e->data->Draw();
	}

	return true;
}

bool Entities::CleanUp()
{
	for (int i = 0; i < entityList.count(); i++)
	{
		ListItem<Entity*>* e = entityList.At(i);
		delete e->data;
	}

	entityList.clear();

	return true;
}

void Entities::AddEntity(fPoint position, Entity::Type type)
{
	Entity* e;
	int r;
	int knifeDirection;

	switch (type)
	{
	case Entity::Type::BAT:
		e = (Entity*)(new Bat((Module*)this, position, batTexture, type, enemySpeed));
		entityList.add(e);
		break;
	case Entity::Type::FRUIT:
		r = rand() % 3;
		e = (Entity*)(new Fruit((Module*)this, position, fruitTexture, type, r));
		entityList.add(e);
		break;
	case Entity::Type::HEART:
		e = (Entity*)(new Heart((Module*)this, position, heartTexture, type));
		entityList.add(e);
		break;
	case Entity::Type::PIG:
		e = (Entity*)(new Pig((Module*)this, position, pigTexture, type, enemySpeed, pigHealth, enemyGravity, enemyJumpForce));
		entityList.add(e);
		break;
	case Entity::Type::KNIFE:
		knifeDirection = app->player->knifeDirection;
		e = (Entity*)(new Knife((Module*)this, position, knifetexture, type, knifeDirection, knifeSpeed));
		entityList.add(e);
		break;
	}
}

void Entities::OnCollision(Collider* a, Collider* b, float dt)
{
	for (int i = 0; i < entityList.count(); i++)
	{
		ListItem<Entity*>* e = entityList.At(i);

		if (e->data->collider == a && b != nullptr)
		{
			e->data->Collision(b);
		}
	}
}

void Entities::ResetEntities()
{
	for (int i = 0; i < entityList.count(); i++)
	{
		ListItem<Entity*>* e = entityList.At(i);

		e->data->Reset();
	}
}
