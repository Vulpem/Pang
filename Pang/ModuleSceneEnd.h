#pragma once
#include "Module.h"
#include "ModuleAnimation.h"
#include "Globals.h"

class ModuleSceneEnd : public Module
{
public:
	ModuleSceneEnd(Application* app, bool start_enabled = true);
	~ModuleSceneEnd();

	bool Start();
	update_status Update();
	bool CleanUp();

	SDL_Texture* graphics;
	SDL_Rect imageRect;

	SDL_Texture* text1Text;
	SDL_Texture* text2Text;

	SDL_Rect text1Rect;
	SDL_Rect text2Rect;
	TTF_Font* font2;

public:
};