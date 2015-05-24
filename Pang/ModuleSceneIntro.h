#pragma once
#include "Module.h"
#include "ModuleAnimation.h"
#include "Globals.h"

enum ui_intro
{
	UI_TEXT1,
	UI_TEXT2,
	UI_TEXT3,
	INTRO_UI_MAX
};

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
	SDL_Texture* timer;
	SDL_Texture* textText;
	TTF_Font* textFont;

	SDL_Texture** uiText;
	SDL_Rect* rectText;

	SDL_Rect textRect;
	SDL_Rect timerRect;
	SDL_Rect selectedRect;

};