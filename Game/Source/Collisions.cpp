#include "Collisions.h"

#include "App.h"
#include "Render.h"
#include "Log.h"
#include "Player.h"

#include "Optick/include/optick.h"

Collisions::Collisions() : Module()
{
	name.Create("collisions");
}

Collisions::~Collisions()
{

}

// Called before render is available
bool Collisions::Awake(pugi::xml_node&)
{
	bool ret = true;
	return ret;
}

// Called before the first frame
bool Collisions::Start()
{
	bool ret = true;
	return ret;
}

// Called each loop iteration
bool Collisions::PreUpdate()
{
	bool ret = true;

	ListItem<Collider*>* start = dynamicColliders.start;

	while (start != nullptr)
	{
		if (start->data->pendingToDelete)
		{
			delete start->data;
			dynamicColliders.del(start);
			start = start->next;
			continue;
		}

		start = start->next;
	}

	start = staticColliders.start;

	while (start != nullptr)
	{
		if (start->data->pendingToDelete)
		{
			delete start->data;
			staticColliders.del(start);
			start = start->next;
			continue;
		}

		start = start->next;
	}

	return ret;
}

bool Collisions::Update(float dt)
{
	OPTICK_EVENT("CollisionsUpdate", Optick::Category::Physics);
	bool ret = true;

	for (int i = 0; i < dynamicColliders.count(); i++)
	{
		for (int j = 0; j < staticColliders.count(); j++)
		{
			if (i >= dynamicColliders.count())
				break;
			if (dynamicColliders[i]->Intersects(staticColliders[j]->rect))
			{
				for each (Module*m in dynamicColliders[i]->listeners)
				{
					if (m == nullptr) break;
					if (j < staticColliders.count() && i < dynamicColliders.count())
						m->OnCollision(dynamicColliders[i], staticColliders[j], dt);
				}
				for each (Module*m in staticColliders[j]->listeners)
				{
					if (m == nullptr) break;
					if (j < staticColliders.count() && i < dynamicColliders.count())
						m->OnCollision(staticColliders[j], dynamicColliders[i], dt);
				}
			}
		}

		for (int j = i; j < dynamicColliders.count(); j++)
		{
			if (i == j)
				continue;
			if (i >= dynamicColliders.count())
				break;
			if (dynamicColliders[i]->Intersects(dynamicColliders[j]->rect))
			{
				for each (Module * m in dynamicColliders[i]->listeners)
				{
					if (m == nullptr) break;
					if (j < dynamicColliders.count() && i < dynamicColliders.count())
						m->OnCollision(dynamicColliders[i], dynamicColliders[j], dt);
				}
				for each (Module * m in dynamicColliders[j]->listeners)
				{
					if (m == nullptr) break;
					if (j < dynamicColliders.count() && i < dynamicColliders.count())
						m->OnCollision(dynamicColliders[j], dynamicColliders[i], dt);
				}
			}
		}
	}

	return ret;
}

bool Collisions::PostUpdate()
{
	OPTICK_EVENT("CollisionsPostUpdate", Optick::Category::Rendering);
	bool ret = true;

	if (showColliders == true)
	{
		for (uint i = 0; i < staticColliders.count(); ++i)
		{
			if (staticColliders[i]->type == Collider::Type::DEATH)
			{
				DrawCollider(&staticColliders[i]->rect, 208, 48, 75, 80);
			}
			else if (staticColliders[i]->type == Collider::Type::ENDLEVEL)
			{
				DrawCollider(&staticColliders[i]->rect, 80, 72, 79, 80);
			}
			else if (staticColliders[i]->type == Collider::Type::ITEMHEALTH)
			{
				DrawCollider(&staticColliders[i]->rect, 48, 56, 171, 80);
			}
			else if (staticColliders[i]->type == Collider::Type::ITEMSCORE)
			{
				DrawCollider(&staticColliders[i]->rect, 255, 145, 0, 80);
			}
			else if (staticColliders[i]->type == Collider::Type::ITEMNUT || staticColliders[i]->type == Collider::Type::SECRETTEXT)
			{
				DrawCollider(&staticColliders[i]->rect, 244, 244, 16, 80);
			}
			else if (staticColliders[i]->type == Collider::Type::CHECKPOINT1 || staticColliders[i]->type == Collider::Type::CHECKPOINT2)
			{
				DrawCollider(&staticColliders[i]->rect, 48, 171, 54, 80);
			}
			else
			{
				DrawCollider(&staticColliders[i]->rect, 59, 198, 191, 80);
			}
		}

		for (uint i = 0; i < dynamicColliders.count(); ++i)
		{
			DrawCollider(&dynamicColliders[i]->rect, 255, 0, 0, 80);
		}
	}

	return ret;
}

// Called before quitting
bool Collisions::CleanUp()
{
	for (int i = 0; i < staticColliders.count(); i++)
	{
		delete staticColliders[i];
	}

	for (int i = 0; i < dynamicColliders.count(); i++)
	{
		if (dynamicColliders[i] != app->player->collider)
			delete dynamicColliders[i];
	}

	staticColliders.clear();
	dynamicColliders.clear();

	return true;
}

Collider* Collisions::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener)
{
	Collider* ret = nullptr;

	ret = new Collider(rect, type, listener);

	if (type == Collider::Type::DYNAMIC || type == Collider::Type::PIG || type == Collider::Type::BAT)
	{
		dynamicColliders.add(ret);
	}
	else
	{
		staticColliders.add(ret);
	}

	return ret;
}

void Collisions::DrawCollider(const SDL_Rect* section, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	app->render->DrawRectangle(*section, r, g, b, a, true, true);
}

void Collisions::RemoveCollider(Collider* collider)
{
	for (int i = 0; i < staticColliders.count(); i++)
	{
		Collider* c = staticColliders[i];
		if (c == collider)
		{
			ListItem<Collider*>* item = staticColliders.At(i);
			staticColliders.del(item);
		}
	}

	for (int i = 0; i < dynamicColliders.count(); i++)
	{
		Collider* c = dynamicColliders[i];
		if (c == collider)
		{
			ListItem<Collider*>* item = dynamicColliders.At(i);
			dynamicColliders.del(item);
		}
	}

	delete collider;
}