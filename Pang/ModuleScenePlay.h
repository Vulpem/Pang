#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"

class ModuleScenePlay : public Module
{
public:
	ModuleScenePlay(Application* app, bool start_enabled = true);
	~ModuleScenePlay();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	
	SDL_Texture* graphics;
	SDL_Rect background;

};