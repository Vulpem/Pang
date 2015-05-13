#pragma once
#include "Module.h"
#include "p2Point.h"
#include "List.h"
#include "Globals.h"


struct Boost
{
	Animation anim;

	p2Point<int> position;

	SDL_Rect rect;

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

	void AddBoost(int x, int y);

public:
	p2List<Boost*> activeBoost;
};