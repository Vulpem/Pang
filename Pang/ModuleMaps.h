#pragma once

#include "Module.h"
#include "Globals.h"

class ModuleMaps : public Module
{
public:
	SDL_Rect tile;
	static int map[26][48];

	ModuleMaps(Application* app, bool start_enabled = true);
	~ModuleMaps();

	bool Start();
	update_status Update();
	bool CleanUp();

	void PrepareMap();

public:

	SDL_Texture* graphics;

	SDL_Rect ladder_l;
	SDL_Rect ladder_m;
	SDL_Rect ladder_r;
};
