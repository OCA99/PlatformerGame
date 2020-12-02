#include "Knife.h"
#include "Render.h"

Knife::Knife(Module* parent, fPoint position, SDL_Texture* texture, Type type) : Entity(parent, position, texture, type)
{

}

bool Knife::Start()
{
	return true;
}

bool Knife::Update(float dt)
{

	return true;
}

bool Knife::Draw()
{

	return true;
}

void Knife::Collision(Collider* other)
{

}