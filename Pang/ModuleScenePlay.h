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

	SDL_Surface* player1Surf;
	SDL_Rect player1Rect;
	int lives;
	int currentLvl;

	bool debugMode = false;

	ModuleScenePlay(Application* app, bool start_enabled = true);
	~ModuleScenePlay();

	bool Init();
	bool Start();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void UpdateInterface();

};