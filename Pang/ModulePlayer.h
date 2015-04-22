#pragma once
#include "Module.h"
#include "ModuleAnimation.h"
#include "Globals.h"
#include "p2Point.h"


enum PlayerStates
{
	standing,
	climbing,
	falling,
};

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	void Kill();

public:

	int playerState = standing;
	int fallCounter = 0;
	bool ladderAlign = false;
	int movementDirection = 1;

	int speed = 2;
	bool undying = true;
	bool dead = false;

	p2Point<int> position;

	SDL_Texture* graphics;

	Animation* current_animation = NULL;

	Animation idle;
	Animation idle2;
	Animation forward;
	Animation backward;
	Animation climb;
	Animation endclimb;
	Animation shot;
	Animation shot2;

private:
	void IsFalling();
	bool MiddleLadder();

	void Movement();
	void Shoot();
	void Fall();
	void Climb();

	void CheckBallCollision();

	bool LadderUpEnds();
	bool LadderDownEnds();
	bool CanClimbUp();
	bool CanClimbDown();
	void AlignLadder(int direction);
	int GetLadderCenter(int direction);
};