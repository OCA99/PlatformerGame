#include "Knife.h"
#include "Bat.h"
#include "Render.h"

Knife::Knife(Module* parent, fPoint position, SDL_Texture* texture_, Type type, int knifeDirection_) : Entity(parent, position, texture, type)
{
	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 25, 6 }), Collider::Type::KNIFE, parent);

	initialPosition = position;

	knifeDirection = knifeDirection_;

	texture = texture_;
}

bool Knife::Start()
{

	position.x = initialPosition.x;

	return true;
}

bool Knife::Update(float dt)
{

	speed = 10;
	position.y = initialPosition.y + 8;
	position.x += speed * knifeDirection;
	
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