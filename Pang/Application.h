#pragma once

#include "List.h"
#include "Globals.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"

#include "ModuleScenePlay.h"
#include "ModuleSceneIntro.h"

#include "ModuleFadeToBlack.h"
#include "ModulePlayer.h"
#include "ModuleGun.h"
#include "ModuleMaps.h"
#include "ModuleBalls.h"
#include "ModuleAudio.h"
#include "ModuleParticles.h"

class Application
{
public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleParticles* particles;
	

	ModuleSceneIntro* backgroundIntro;
	ModuleScenePlay* backgroundPlay;

	ModuleFadeToBlack* fade;
	ModulePlayer* player;
	ModuleGun* gun;
	ModuleMaps* maps;
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