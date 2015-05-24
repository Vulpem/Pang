#pragma once
#include "Module.h"
#include "ModuleAnimation.h"
#include "Globals.h"

enum UI_Intro_intro
{

	UI_Intro_MTFUJI = 1,
	UI_Intro_MTKEIRIN,
	UI_Intro_EMERALD,
	UI_Intro_ANKOR_WATT,
	UI_Intro_AUSTRALIA,
	UI_Intro_TAJ_MAHAL,
	UI_Intro_LENINGRAD,
	UI_Intro_PARIS,
	UI_Intro_LONDON,
	UI_Intro_BARCELONA,
	UI_Intro_ATHENS,
	UI_Intro_EGYPT,
	UI_Intro_KENYA,
	UI_Intro_NEW_YORK,
	UI_Intro_MAYA,
	UI_Intro_ANTARTICA,
	UI_Intro_EASTER,
	UI_Intro_ISLAND,
	UI_Intro_TEMPLE,
	UI_Intro_TEXT1,
	UI_Intro_TEXT2,
	UI_Intro_TEXT3,
	UI_Intro_CHECKMARK,
	UI_Intro_STAGE,
	UI_Intro_MAX
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