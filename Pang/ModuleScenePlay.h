#pragma once
#include "Module.h"
#include "ModuleAnimation.h"
#include "Globals.h"

class ModuleScenePlay : public Module
{
public:
	SDL_Texture* graphics;
	SDL_Texture* livesGraphics;
	SDL_Rect livesRect;
	SDL_Rect background;
	SDL_Rect interfaceRect;

	int lives;

	bool debugMode = false;

	ModuleScenePlay(Application* app, bool start_enabled = true);
	~ModuleScenePlay();

	bool Start();
	update_status Update();
	bool CleanUp();

	void UpdateInterface();

};