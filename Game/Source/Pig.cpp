#include "Pig.h"
#include "App.h"
#include "Render.h"
#include "Player.h"
#include "Pathfinding.h"
#include "Map.h"
#include "Debug.h"
#include "Log.h"
#include "Collisions.h"
#include "Audio.h"

#include <math.h>

Pig::Pig(Module* parent, fPoint position, SDL_Texture* texture, Type type, int s, int h, int g, int jForce) : Entity(parent, position, texture, type)
{
	idleLeftAnimation.GenerateAnimation(SDL_Rect({ 0, 0, 324, 30 }), 1, 9);
	idleLeftAnimation.speed = 10.0f;

	idleRightAnimation.GenerateAnimation(SDL_Rect({ 0, 30, 324, 30 }), 1, 9);
	idleRightAnimation.speed = 10.0f;

	walkingLeftAnimation.GenerateAnimation(SDL_Rect({ 0, 60, 576, 30 }), 1, 16);
	walkingLeftAnimation.speed = 20.0f;

	walkingRightAnimation.GenerateAnimation(SDL_Rect({ 0, 90, 576, 30 }), 1, 16);
	walkingRightAnimation.speed = 20.0f;

	runningLeftAnimation.GenerateAnimation(SDL_Rect({ 0, 120, 432, 30 }), 1, 12);
	runningLeftAnimation.speed = 22.0f;

	runningRightAnimation.GenerateAnimation(SDL_Rect({ 0, 150, 432, 30 }), 1, 12);
	runningRightAnimation.speed = 22.0f;

	hitLeftAnimation.GenerateAnimation(SDL_Rect({ 0, 180, 180, 30 }), 1, 5);
	hitLeftAnimation.speed = 10.0f;
	hitLeftAnimation.loop = false;

	hitRightAnimation.GenerateAnimation(SDL_Rect({ 0, 210, 180, 30 }), 1, 5);
	hitRightAnimation.speed = 10.0f;
	hitRightAnimation.loop = false;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y - 12, 16, 28 }), Collider::Type::PIG, parent);

	speed = s;
	health = h;

	gravity = g;
	jumpForce = jForce;

	lastPlayerPosition.x = -1;
	lastPlayerPosition.y = -1;

	lastPosition = iPoint((int)position.x / app->map->data.tileWidth, (int)position.y / app->map->data.tileHeight);

	currentAnimation = &idleLeftAnimation;
	state = State::IDLE;
}

bool Pig::Start()
{
	return true;
}

void Pig::CleanUp()
{
	collider->pendingToDelete = true;
}

bool Pig::Update(float dt)
{
	UpdatePathfinding(dt);

	currentAnimation->Update(dt);

	switch (state)
	{
	case State::IDLE:
		if (!lookingRight)
			currentAnimation = &idleLeftAnimation;
		else
			currentAnimation = &idleRightAnimation;
		break;
	case State::WALKING:
		if (health > 1)
		{
			if (!lookingRight)
				currentAnimation = &walkingLeftAnimation;
			else
				currentAnimation = &walkingRightAnimation;
		}
		else
		{
			if (!lookingRight)
				currentAnimation = &runningLeftAnimation;
			else
				currentAnimation = &runningRightAnimation;
		}
		break;
	case State::HIT:
		if (!lookingRight)
			currentAnimation = &hitLeftAnimation;
		else
			currentAnimation = &hitRightAnimation;
		break;
	case State::LOST:
		if (!lookingRight)
			currentAnimation = &idleLeftAnimation;
		else
			currentAnimation = &idleRightAnimation;
		break;
	case State::DYING:
		currentAnimation = &deathAnimation;
	}

	return true;
}

void Pig::UpdatePathfinding(float dt)
{
	iPoint playerPos;
	playerPos.x = (app->player->collider->rect.x) / app->map->data.tileWidth;
	playerPos.y = (app->player->collider->rect.y) / app->map->data.tileHeight;

	iPoint gridPos;
	gridPos.x = position.x / app->map->data.tileWidth;
	gridPos.y = position.y / app->map->data.tileHeight;

	bool changedPosition = (abs(gridPos.x - lastPosition.x) > 2 && !jumping);
	//changedPosition = false;
	iPoint groundPos;
	groundPos = app->pathfinding->GetGroundTile(playerPos);

	if ((changedPosition || groundPos != lastPlayerPosition) && playerPos.DistanceTo(gridPos) <= 16 && state != State::DYING && !app->player->godMode && !jumping)
	{
		lastPlayerPosition = groundPos;


		int n = app->pathfinding->CreatePath(gridPos, groundPos, true, 3, 20);
		if (n == -1)
		{
			hasPath = false;
			path.Clear();
			path.PushBack(iPoint(gridPos.x, gridPos.y));
		}
		else
		{
			hasPath = true;
			const DynArray<iPoint>* newPath = app->pathfinding->GetLastPath();
			int j = 0;
			for (int i = 0; i < path.Count(); i++)
			{
				if (path[i] != (*newPath)[j])
					continue;
				j++;
				break;
			}
			path.Clear();
			for (int i = 0; i < newPath->Count(); i++)
			{
				path.PushBack((*newPath)[i]);
			}
			if (j < path.Count())
				pathIndex = j;
			else
				pathIndex = 0;
		}
	}

	if (hasPath)
	{
		if (pathIndex < path.Count())
		{
			state = State::WALKING;

			iPoint pixelPos = iPoint(path[pathIndex].x * app->map->data.tileWidth, path[pathIndex].y * app->map->data.tileHeight);

			while (pixelPos.x == (int)position.x/* && (abs(pixelPos.y - (int)position.y) <= 12 || jumping)*/)
			{
				pathIndex++;
				if (pathIndex < path.Count())
					pixelPos = iPoint(path[pathIndex].x * app->map->data.tileWidth, path[pathIndex].y * app->map->data.tileHeight);
				else
					break;
			}

			if (pixelPos.x > position.x)
			{
				if (abs(position.x - pixelPos.x) < speed * dt)
					position.x += pixelPos.x - position.x;
				else
				{
					lookingRight = true;
					SafeMovementX(speed * dt);
				}
			}

			if (pixelPos.x < position.x)
			{
				if (abs(position.x - pixelPos.x) < speed * dt)
					position.x -= position.x - pixelPos.x;
				else
				{
					lookingRight = false;
					SafeMovementX(-speed * dt);
				}
			}

			iPoint nextPixelPos;
			if (pathIndex + 1 < path.Count())
				nextPixelPos = iPoint(path[pathIndex + 1].x * app->map->data.tileWidth, path[pathIndex + 1].y * app->map->data.tileHeight);
			else
				nextPixelPos = pixelPos;



			bool nextNoGround = false;
			if (pathIndex + 1 < path.Count() && pathIndex + 2 < path.Count())
			{
				iPoint floor = app->pathfinding->GetGroundTile(path[pathIndex + 1]);
				if (path[pathIndex + 1] != floor && abs(path[pathIndex].x * app->map->data.tileWidth - path[pathIndex + 1].x * app->map->data.tileWidth) < 4 && path[pathIndex + 2].y <= path[pathIndex].y)
					nextNoGround = true;
			}

			if (((pixelPos.y <= (int)position.y - 16 && nextPixelPos.y <= (int)position.y - 16) || nextNoGround) && !jumping)
			{
				verticalVelocity = jumpForce;
				jumping = true;
			}
		}
		else
			state = State::IDLE;
	}
	else
	{
		state = State::IDLE;
	}

	verticalVelocity -= gravity * dt;

	SafeMovementY(-verticalVelocity * dt);
	collider->SetPos((int)position.x, (int)position.y - 12);

	if (changedPosition)
		lastPosition = gridPos;
}

bool Pig::Draw()
{
	app->render->DrawTexture(texture, position.x - 10, position.y - 14, &currentAnimation->GetCurrentFrame());
	
	if (hasPath && app->debug->showPaths)
	{
		if (pathIndex < path.Count())
			app->render->DrawRectangle(SDL_Rect({ path[pathIndex].x * app->map->data.tileWidth - 3 + app->map->data.tileWidth / 2, path[pathIndex].y * app->map->data.tileHeight - 3 + app->map->data.tileHeight / 2, 6, 6 }), 255, 0, 0);
		app->pathfinding->DrawPath(&path, 255, 0, 0);
	}


	return true;
}

void Pig::Collision(Collider* other)
{
	if (other == app->player->collider)
	{
		
	}

	if (other->type == Collider::Type::STATIC || other->type == Collider::Type::DEATH)
	{
		int deltaX = collider->rect.x - other->rect.x;
		int deltaY = collider->rect.y - other->rect.y;

		if (std::abs(deltaX) > std::abs(deltaY))
		{
			if (deltaX > 0)
			{
				position.x += other->rect.x + other->rect.w - collider->rect.x;
			}
			else
			{
				position.x -= collider->rect.x + collider->rect.w - other->rect.x;
			}
		}
		else
		{
			if (deltaY > 0)
			{
				verticalVelocity = 0.0f;
				position.y += other->rect.y + other->rect.h - collider->rect.y;
			}
			else
			{
				if (verticalVelocity < 0.0f)
				{
					jumping = false;
					verticalVelocity = 0.0f;
					position.y -= collider->rect.y + collider->rect.h - other->rect.y;
				}
			}
		}
		collider->SetPos((int)position.x, (int)position.y - 12);
	}
}

int Pig::GetJumpFrameCount(int deltaY)
{
	if (deltaY <= 0)
		return 0;
	else
	{
		switch (deltaY)
		{
		case 1:
			return 1;
		case 2:
			return 2;
		case 3:
			return 5;
		case 4:
			return 8;
		default:
			return 14;
		}
	}
}

void Pig::SafeMovementX(float deltaX)
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

void Pig::SafeMovementY(float deltaY)
{
	float initialY = position.y;

	int yDir = 0;
	if (deltaY > 0.0f)
		yDir = 1;
	else if (deltaY < 0.0f)
		yDir = -1;

	for (int y = 1; y <= abs((int)deltaY); y++)
	{
		position.y = initialY + (float)(y * yDir);
		collider->SetPos(position.x, position.y - 12);
		if (app->map->IntersectsWithMap(collider, 2))
		{
			break;
		}
	}

	float remainder = deltaY - (float)((int)deltaY);
	position.y += remainder;
}