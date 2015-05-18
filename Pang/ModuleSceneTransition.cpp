#include "Globals.h"
#include "Application.h"
#include "ModuleSceneTransition.h"

ModuleSceneTransition::ModuleSceneTransition(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	graphics = NULL;
	graphics2 = NULL;
	timeCounter = 0;
}

ModuleSceneTransition::~ModuleSceneTransition()
{

}

bool ModuleSceneTransition::Start(int _nextLevel)
{
	nextLevel = _nextLevel;
	LOG("Scene Transition");
	bool ret = true;
	graphics = App->textures->Load("./Image_Sources/TransitionScene.png");
	return ret;
}

bool ModuleSceneTransition::CleanUp()
{
	LOG("--Cleanup Transition scene")
		App->textures->Unload(graphics);

	return true;
}

// Update: draw background
update_status ModuleSceneTransition::Update()
{
	// Draw everything --------------------------------------
	timeCounter++;
	if (timeCounter >= 150)
	{
		timeCounter = 0;
		App->backgroundPlay->Enable(nextLevel);
		Disable();
	}
	/*
	if (timeCounter / 25 % 2 == 0)
		App->renderer->Blit(graphics, 0, 0, NULL);
	else
		App->renderer->Blit(graphics2, 0, 0, NULL);

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
	{
		App->fade->FadeToBlack(this, App->backgroundPlay, 3.0f);
	}
	*/
	App->renderer->Blit(graphics, 0, 0, NULL);

	return UPDATE_CONTINUE;
}

void ModuleSceneTransition::Enable(int nextLevel)
{
	if (enabled == false)
	{
		enabled = true;
		Start(nextLevel);
	}
}
