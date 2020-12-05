#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"

struct Animation;
struct Collider;
struct SDL_Texture;

enum PlayerState
{
	IDLE,
	RUNNING,
	JUMPING,
	DOUBLE_JUMPING,
	FALLING,
	PREPARE_TO_SPAWN,
	SPAWNING,
	DASHING,
	DYING
};

class Player : public Module
{

public:
	PlayerState playerState = PlayerState::PREPARE_TO_SPAWN;
	bool isDead;

	Player();
	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	void OnCollision(Collider* a, Collider* b, float dt);

	void UpdateState(float dt);
	void UpdateLogic(float dt);
	void ChangeState(PlayerState previous, PlayerState next);
	void GodMovement(float dt);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);

	void Reload();

	bool xCausesCollision(Collider& other, float dt);
	bool yCausesCollision(Collider& other, float dt);

	void SafeMovementX(float deltaX);
	void SafeMovementY(float deltaY);

	Animation idleRightAnim;
	Animation idleLeftAnim;
	Animation runRightAnim;
	Animation runLeftAnim;
	Animation jumpRightAnim;
	Animation jumpLeftAnim;
	Animation doubleJumpRightAnim;
	Animation doubleJumpLeftAnim;
	Animation fallRightAnim;
	Animation fallLeftAnim;
	Animation prepareToSpawnAnim;
	Animation appearAnim;
	Animation disappearRightAnim;
	Animation disappearLeftAnim;

	Point<float> position;

	bool godMode = false;
	float gravity;
	float verticalVelocity = 0.0f;

	int health;

	Point<float> initialPosition;
	Point<float> respawnPosition;

	Point<float> checkpoint1Position;
	Point<float> checkpoint2Position;
	bool unlockedChekpoint1 = false;
	bool unlockedChekpoint2 = false;

	int knifeDirection;

	bool gravityOn = false;

	uint gameStartFx = 0;
	const char* gameStartFxPath;

	float cooldown = 3.0f;
	float maxCooldown = 3.0f;

	float knifeCooldown = 0.75f;
	float maxKnifeCooldown = 0.75f;


	Collider* collider;
	float jumpForce;

	uint doubleJumpFx = 0;

	bool saveOnce1;
	bool saveOnce2;

private:
	SDL_Texture* texture = nullptr;
	Animation* currentAnim = nullptr;

	bool isGoingRight = false;
	unsigned int jumpsLeft = 2;

	float initialWaitCount = 0.0f;
	float initialWait = 0.1f;

	int speed;
	float maxVerticalVelocity = 850.0f;

	float initialImpulse = 0.0f;
	float dashLength = 1.0f;
	float impulse = 0.0f;
	float impulseAcceleration = 0.0f;
	int frameCounter = 0;
	bool dashing;

	int maxJumps = 2;
	int availableJumps = 2;


	bool canMoveRight = true;
	bool canMoveLeft = true;
	bool canMoveUp = true;
	bool canMoveDown = true;

	int xDirection = 0;
	int yDirection = 0;

	const char* texturePath;

	uint jumpFx = 0;
	uint gameOverFx = 0;
	uint pickUpFruitFx = 0;
	uint checkpointFx = 0;
	uint dashFx = 0;
	uint throwKnifeFx = 0;
	uint pickUpHeartFx = 0;

	const char* jumpFxPath;
	const char* doubleJumpFxPath;
	const char* gameOverFxPath;
	const char* pickUpFruitFxPath;
	const char* checkpointFxPath;
	const char* dashFxPath;
	const char* throwKnifeFxPath;
	const char* pickUpHeartFxPath;

	uint nutsFx = 0;
	const char* nutsFxPath;
};


#endif
