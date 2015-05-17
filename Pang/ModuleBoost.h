#pragma once
#include "Module.h"
#include "p2Point.h"
#include "List.h"
#include "Globals.h"


class Boost
{
public:

	int type;
	int lifeTime;
	Animation anim;

	p2Point<int> position;

	void Fall(Application* app);
	bool Update(Application* app);
};

class ModuleBoost : public Module
{
public:
	ModuleBoost(Application* app, bool start_enabled = true);
	~ModuleBoost();

public:
	bool Start();
	update_status Update();
	bool CleanUp();

	void AddBoost(int x, int y, Boosts boostType);

public:
	SDL_Rect playerBoost;
	p2List<Boost*> activeBoost;
	SDL_Texture* graphics;
	unsigned int pickedUp;
};