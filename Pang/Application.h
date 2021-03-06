#pragma once

#include "List.h"
//#include "Globals.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"

#include "ModuleScenePlay.h"
#include "ModuleSceneIntro.h"
#include "ModuleSceneTransition.h"
#include "ModuleSceneEnd.h"

#include "ModulePlayer.h"
#include "ModulePlayer2.h"
#include "ModuleGun.h"
#include "ModuleMaps.h"
#include "ModuleBalls.h"
#include "ModuleAudio.h"
#include "ModuleParticles.h"
#include "ModuleBoost.h"
#include "ModuleFonts.h"

class Application
{
public:
	ModuleRender* render;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleFonts* fonts;	

	ModuleSceneIntro* sceneIntro;
	ModuleScenePlay* scenePlay;
	ModuleSceneTransition* sceneTransition;
	ModuleSceneEnd* sceneEnd;

	ModulePlayer* player;
	ModulePlayer2* player2;
	ModuleGun* gun;
	ModuleMaps* maps;
	ModuleBalls* balls;
	ModuleParticles* particles;
	ModuleBoost* boosts;



	int frames;
private:
	
	int lastTime;
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