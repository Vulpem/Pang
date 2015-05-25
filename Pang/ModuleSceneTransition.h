#pragma once
#include "Module.h"
#include "Globals.h"

enum UI_Transition_messages
{
	UI_Transition_MTFUJI = 1,
	UI_Transition_MTKEIRIN,
	UI_Transition_EMERALD,
	UI_Transition_ANKOR_WATT,
	UI_Transition_AUSTRALIA,
	UI_Transition_TAJ_MAHAL,
	UI_Transition_LENINGRAD,
	UI_Transition_PARIS,
	UI_Transition_LONDON,
	UI_Transition_BARCELONA,
	UI_Transition_ATHENS,
	UI_Transition_EGYPT,
	UI_Transition_KENYA,
	UI_Transition_NEW_YORK,
	UI_Transition_MAYA,
	UI_Transition_ANTARTICA,
	UI_Transition_EASTER,
	UI_Transition_ISLAND,
	UI_Transition_TEMPLE,
	UI_Transition_STAGE,
	UI_Transition_CHECKMARK,
	UI_Transition_TIMEBONUS,
	UI_Transition_PTS,
	UI_Transition_NEXTEXTEND,
	UI_Transition_PUSHBUTTON,
	UI_Transition_MAX,
};
class ModuleSceneTransition : public Module
{
public:
	ModuleSceneTransition(Application* app, bool start_enabled = true);
	~ModuleSceneTransition();

	bool Init();
	bool Start(int nextLevel);
	update_status Update();
	bool CleanUp();

	void Enable(int nextLevel);
	void PrintStats();
	void PrintMapInterface();

	int timeCounter;
	int nextLevel;


public:

	SDL_Texture** uiText;
	SDL_Rect* rectText;

	SDL_Texture* graphics;
	SDL_Texture* graphics2;
	SDL_Texture* selected;

	SDL_Rect textRect;
	SDL_Rect imageRect;
	SDL_Rect selectedRect;


};