#pragma once
#include "Module.h"
#include "ModuleAnimation.h"
#include "Globals.h"

class ModuleScenePlay : public Module
{
public:
	SDL_Texture* graphics;
	SDL_Rect background;
	SDL_Rect interfaceRect;

	ModuleScenePlay(Application* app, bool start_enabled = true);
	~ModuleScenePlay();

	bool Start();
	update_status Update();
	bool CleanUp();

};