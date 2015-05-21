#pragma once
#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	renderer = new ModuleRender(this);
	textures = new ModuleTextures(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this);
//	fonts = new ModuleFonts(this, true);

	sceneIntro = new ModuleSceneIntro(this, true);
	scenePlay = new ModuleScenePlay(this, false);
	sceneTransition = new ModuleSceneTransition(this, false);


	player = new ModulePlayer(this, false);
	gun = new ModuleGun(this, false);
	maps = new ModuleMaps(this, false);
	balls = new ModuleBalls(this, false);
	particles = new ModuleParticles(this);
	boosts = new ModuleBoost(this, false);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order
	AddModule(window);
	AddModule(renderer);
	AddModule(textures);
	AddModule(input);
	AddModule(audio);
//	AddModule(fonts);
	
	AddModule(sceneIntro);
	AddModule(scenePlay);
	AddModule(sceneTransition);

	AddModule(maps);
	
	AddModule(gun);
	AddModule(player);
	AddModule(balls);
	AddModule(particles);
	AddModule(boosts);
}

Application::~Application()
{
	delete boosts;
	delete particles;
	delete balls;
	delete player;
	delete gun;
	delete maps;
	
	delete sceneTransition;
	delete scenePlay;
	delete sceneIntro;
	
//	delete fonts;
	delete audio;
	delete input;
	delete textures;
	delete renderer;
	delete window;

}

bool Application::Init()
{
	frames = 0;
	bool ret = true;

	// Call Init() in all modules
	p2List_item<Module*>* item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->Init();
		item = item->next;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		if (item->data->IsEnabled())
			ret = item->data->Start();
		item = item->next;
	}
	
	return ret;
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	p2List_item<Module*>* item = list_modules.getFirst();

	// CAPING FPS IN THE UPDATE///////////////////
	int time = SDL_GetTicks();
	int delta = time - lastTime;

	double deltaSeconds = delta / 1000.0f; //ms to sec

	if (deltaSeconds >= 1.0 / FPS)
	{
		lastTime = time;
		frames++;
		//////////////////////////////////////////////

		while (item != NULL && ret == UPDATE_CONTINUE)
		{
			if (item->data->IsEnabled())
				ret = item->data->PreUpdate();
			item = item->next;
		}


		item = list_modules.getFirst();

		while (item != NULL && ret == UPDATE_CONTINUE)
		{
			if (item->data->IsEnabled())
				ret = item->data->Update();
			item = item->next;
		}

		
		item = list_modules.getFirst();

		while (item != NULL && ret == UPDATE_CONTINUE)
		{
			if (item->data->IsEnabled())
				ret = item->data->PostUpdate();
			item = item->next;
		}
	}
	return ret;
}

bool Application::CleanUp()
{
	LOG("Application Cleanup");
	bool ret = true;
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.add(mod);
}