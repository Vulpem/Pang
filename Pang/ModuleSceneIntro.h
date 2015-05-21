#pragma once
#include "Module.h"
#include "ModuleAnimation.h"
#include "Globals.h"

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

	int SelectedPosition(bool x);


	int timeCounter;
	bool mapOn;
	int nextLevel;
	bool paused;



public:
	
	SDL_Texture* graphics;
	SDL_Texture* graphics2;

	SDL_Texture* map;
	SDL_Texture* selected;
	SDL_Texture* selected2;
	SDL_Texture* timer;

	SDL_Rect interfaceRect;
	SDL_Rect timerRect;
	SDL_Rect selectedRect;

};