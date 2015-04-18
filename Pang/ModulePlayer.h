#pragma once
#include "Module.h"
#include "Animation.h"
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
	ModulePlayer(Application* app);
	~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	void CheckBallCollision();
	void Kill();

	int deaths = 0;
	bool dead = false;
	int playerState = standing;
	int fallCounter = 0;
	bool ladderAlign = false;
	int movementDirection = 1;
	p2Point<int> position;

public:

	Animation* current_animation = NULL;

	SDL_Texture* graphics;

	Animation idle;
	Animation idle2;
	Animation forward;
	Animation backward;
	Animation climb;
	Animation endclimb;
	Animation shot;
	Animation shot2;



};