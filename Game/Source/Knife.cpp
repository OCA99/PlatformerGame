#include "Knife.h"
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
	
	collider->SetPos(position.x, position.y);

	return true;
}

bool Knife::Draw()
{

	app->render->DrawTexture(texture, position.x, position.y, NULL);

	return true;
}

void Knife::Collision(Collider* other)
{

}