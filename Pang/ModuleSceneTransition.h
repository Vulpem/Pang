#pragma once
#include "Module.h"
#include "Globals.h"

enum UI_Transition_messages
{
	UI_Transition_STAGE,
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

	int timeCounter;
	int nextLevel;


public:

	SDL_Texture** uiText;
	SDL_Rect* rectText;

	SDL_Texture* graphics;
	SDL_Texture* graphics2;
	SDL_Texture* textText;
	SDL_Rect textRect;
	SDL_Rect imageRect;


};