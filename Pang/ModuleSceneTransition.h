#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleSceneTransition : public Module
{
public:
	ModuleSceneTransition(Application* app, bool start_enabled = true);
	~ModuleSceneTransition();

	bool Start(int nextLevel);
	update_status Update();
	bool CleanUp();

	void Enable(int nextLevel);

	int timeCounter;
	int nextLevel;


public:

	SDL_Texture* graphics;
	SDL_Surface* textSurf;
	SDL_Rect textRect;
	SDL_Rect imageRect;


};