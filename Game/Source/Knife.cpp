#include "Knife.h"
#include "Render.h"
#include "App.h"
#include "Map.h"

Knife::Knife(Module* parent, fPoint position, SDL_Texture* texture_, Type type, int knifeDirection_) : Entity(parent, position, texture, type)
{
	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 25, 6 }), Collider::Type::KNIFE, parent);

	initialPosition = position;

	knifeDirection = knifeDirection_;

	texture = texture_;
	
	speed = 600;
}

bool Knife::Start()
{


	return true;
}

bool Knife::Update(float dt)
{
	position.y = initialPosition.y + 16;
	SafeMovementX(speed * knifeDirection * dt);
	
	if (knifeDirection == 1)
	{
		knifeRect = SDL_Rect({ 0, 0, 25, 6 });
	}
	else
	{
		knifeRect = SDL_Rect({ 25, 0, 25, 6 });
	}

	collider->SetPos(position.x, position.y);

	return true;
}

bool Knife::Draw()
{

	app->render->DrawTexture(texture, position.x, position.y, &knifeRect);

	return true;
}

void Knife::Collision(Collider* other)
{

	switch (other->type)
	{

	case(Collider::Type::STATIC):
		pendingToDelete = true;
		collider->pendingToDelete = true;
		break;

	case(Collider::Type::BAT):
		pendingToDelete = true;
		collider->pendingToDelete = true;
		break;

	case(Collider::Type::PIG):
		pendingToDelete = true;
		collider->pendingToDelete = true;
		break;

	default:
		break;
	}

}

void Knife::SafeMovementX(float deltaX)
{

	float initialX = position.x;

	int xDir = 0;
	if (deltaX > 0.0f)
		xDir = 1;
	else if (deltaX < 0.0f)
		xDir = -1;

	for (int x = 1; x <= abs((int)deltaX); x++)
	{
		position.x = initialX + (float)(x * xDir);
		collider->SetPos(position.x, position.y - 12);
		if (app->map->IntersectsWithMap(collider, 1))
		{
			break;
		}
	}

	float remainder = deltaX - (float)((int)deltaX);
	position.x += remainder;
}