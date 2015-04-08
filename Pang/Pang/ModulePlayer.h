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

	int playerState = standing;
	int fallCounter = 0;
	bool ladderAlign = false;

public:
	Animation* current_animation = NULL;

	SDL_Texture* graphics;

	Animation idle;
	Animation forward;
	Animation backward;
	Animation climb;
	Animation endclimb;
	Animation shot;
	p2Point<int> position;

};