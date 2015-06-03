#pragma once
#include "Module.h"
#include "ModuleAnimation.h"
#include "Globals.h"

class ModuleScenePlay : public Module
{
public:


	SDL_Texture* backgroundGraphics;
	SDL_Texture* livesGraphics;
	SDL_Texture* timerImage;
	SDL_Texture* ready;

	SDL_Texture* timerNum;


	SDL_Rect lives1Rect;
	SDL_Rect lives2Rect;
	SDL_Rect background;
	SDL_Rect interfaceRect;
	SDL_Rect timerRect;
	SDL_Rect readyRect;

	SDL_Rect timerNumRect1;
	SDL_Rect timerNumRect2;
	SDL_Rect timerNumRect3;

	SDL_Rect player1Rect;
	int lives1;
	int lives2;
	int currentLvl;
	int delay;
	int timer;
	int startTimerEvent;
	int timeBonus;

	bool player2Enabled;

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