#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"

class ModuleFadeToBlack : public Module
{
public:
	ModuleFadeToBlack(Application* app, bool start_enabled = true);
	~ModuleFadeToBlack();

	update_status Update();
	void FadeToBlack(Module* module_off, Module* module_on, float time = 2.0f);

private:

	Uint32 start_time;
	Uint32 total_time;
	SDL_Rect screen;
	bool fading_in;
	Module* mod_on;
	Module* mod_off;
};