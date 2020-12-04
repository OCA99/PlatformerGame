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

#include "Optick/include/optick.h"

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

	batTexturePath = config.child("batTexture").child_value();
	pigTexturePath = config.child("pigTexture").child_value();
	fruitTexturePath = config.child("fruitTexture").child_value();
	heartTexturePath = config.child("heartTexture").child_value();
	knifeTexturePath = config.child("knifeTexture").child_value();

	return true;
}

bool Entities::Load(pugi::xml_node& savedGame)
{
	pugi::xml_node ents = savedGame.child("entities");

	pugi::xml_node e;

	for (e = ents.child("entity"); e; e = e.next_sibling("entity"))
	{
		int type = e.attribute("type").as_int();
		float x = e.attribute("x").as_float();
		float y = e.attribute("y").as_float();
		fPoint position = fPoint(x, y);
		int r;
		Entity* en;
		int dir = 1;
		switch (type)
		{
		case 0:
			r = rand() % 3;
			en = (Entity*)(new Fruit((Module*)this, position, fruitTexture, Entity::Type::FRUIT, r));
			entityList.add(en);
			break;
		case 1:
			en = (Entity*)(new Heart((Module*)this, position, heartTexture, Entity::Type::HEART));
			entityList.add(en);
			break;
		case 2:
			dir = e.attribute("direction").as_int();
			en = (Entity*)(new Knife((Module*)this, position, knifetexture, Entity::Type::KNIFE, dir, knifeSpeed));
			entityList.add(en);
			break;
		case 3:
			en = (Entity*)(new Bat((Module*)this, position, batTexture, Entity::Type::BAT, enemySpeed));
			entityList.add(en);
			break;
		case 4:
			int h = e.attribute("health").as_int();
			en = (Entity*)(new Pig((Module*)this, position, pigTexture, Entity::Type::PIG, enemySpeed, h, enemyGravity, enemyJumpForce));
			entityList.add(en);
			break;
		}
	}

	return true;
}

bool Entities::Save(pugi::xml_node& savedGame)
{
	pugi::xml_node ents = savedGame.append_child("entities");

	for (int i = 0; i < entityList.count(); i++)
	{
		Entity* e = entityList[i];
		pugi::xml_node eNode = ents.append_child("entity");
		pugi::xml_attribute x = eNode.append_attribute("x");
		x.set_value(e->position.x);
		pugi::xml_attribute y = eNode.append_attribute("y");
		y.set_value(e->position.y);
		pugi::xml_attribute type = eNode.append_attribute("type");
		type.set_value((int)e->type);
		pugi::xml_attribute h;
		Pig* p;
		switch (e->type)
		{
		case Entity::Type::PIG:
			p = (Pig*)e;
			h = eNode.append_attribute("health");
			h.set_value(p->health);
			break;
		case Entity::Type::KNIFE:
			Knife* k = (Knife*)e;
			pugi::xml_attribute dir = eNode.append_attribute("direction");
			dir.set_value(k->knifeDirection);
			break;
		}
	}

	return true;
}

bool Entities::Start()
{
	batTexture = app->tex->Load(batTexturePath);
	pigTexture = app->tex->Load(pigTexturePath);
	fruitTexture = app->tex->Load(fruitTexturePath);
	heartTexture = app->tex->Load(heartTexturePath);
	knifetexture = app->tex->Load(knifeTexturePath);

	srand(time(NULL));

	return true;
}

bool Entities::PreUpdate()
{
	return true;
}

bool Entities::Update(float dt)
{
	OPTICK_EVENT("EntitiesUpdate", Optick::Category::GameLogic);

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
	OPTICK_EVENT("EntitiesPostUpdate", Optick::Category::Rendering);

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
