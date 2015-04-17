#pragma once

#include "List.h"
#include "Globals.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleBackground.h"
#include "ModulePlayer.h"
#include "ModuleGun.h"
#include "ModuleMaps.h"
#include "ModuleBalls.h"

class Application
{
public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleBackground* background;
	ModulePlayer* player;
	ModuleGun* gun;
//	ModuleMaps* maps;
	ModuleBalls* balls;

private:

	p2List<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
};