#include "Globals.h"
#include "Application.h"
#include "ModuleSceneTransition.h"

ModuleSceneTransition::ModuleSceneTransition(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	graphics = NULL;
	timeCounter = 0;
}

ModuleSceneTransition::~ModuleSceneTransition()
{

}

bool ModuleSceneTransition::Start(int _nextLevel)
{
	textRect = { 0, 0, 0, 16 };
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
	if (timeCounter >= 150 || (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP && timeCounter >= 50))
	{
		timeCounter = 0;
		App->backgroundPlay->Enable(nextLevel);
		Disable();
	}
	
	if (timeCounter / 25 % 2 == 0)
	{
		App->fonts->PrintText("HELLO :)", textSurf, textRect, 150, 100);
	}


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
