#pragma once
#include "Module.h"
#include "ModuleAnimation.h"
#include "Globals.h"

class ModuleScenePlay : public Module
{
public:


	SDL_Texture* backgroundGraphics;
	SDL_Texture* livesGraphics;
	SDL_Texture* timer;


	SDL_Rect livesRect;
	SDL_Rect background;
	SDL_Rect interfaceRect;
	SDL_Rect timerRect;

	SDL_Surface* player1Surf;
	SDL_Rect player1Rect;
	int lives;
	int currentLvl;
	int delay = 0;

	bool debugMode = false;

	ModuleScenePlay(Application* app, bool start_enabled = true);
	~ModuleScenePlay();

	bool Init();
	bool Start(int level);
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void Enable(int level);

	void UpdateInterface();

};