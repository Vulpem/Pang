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
	textRect = { 0, 0, 0, 9 };
	nextLevel = _nextLevel;
	imageRect = { 0, 0, 194, 96 };
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
	if (timeCounter >= 300 || (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP && timeCounter >= 100))
	{
		timeCounter = 0;
		if (nextLevel <= 3)
		{
			App->backgroundPlay->Enable(nextLevel);
			Disable();
		}
		else
		{
			App->backgroundIntro->Enable();
			Disable();
		}
	}
	App->renderer->DrawQuad({ 0, 0, 384, 240 }, 8, 8, 8, 255);
	
	//Print level stats
	App->fonts->PrintNumbers(nextLevel - 1, textSurf, textRect, 170, 145);
	App->fonts->PrintText("STAGE", textSurf, textRect, 170 + 4, 145);

	App->fonts->PrintText("TIME BONUS", textSurf, textRect, 100, 165);
	App->fonts->PrintText("PTS", textSurf, textRect, 260 + 4, 165);
	if (timeCounter > 50)
	{
		App->fonts->PrintText("NEXT EXTEND", textSurf, textRect, 100, 185);
		App->fonts->PrintText("PTS", textSurf, textRect, 260 + 4, 185);
	}
	///////////////////
	//Print push button
	if (timeCounter / 20 % 2 == 0)
	{
		App->fonts->PrintText("PUSH BUTTON", textSurf, textRect, 280, 220);
	}
	//////////////////

	//Print background
	imageRect.y = 0 + (nextLevel - 2) * 96;
	if (timeCounter / 35 % 2 == 0)
		{
			imageRect.x = 0;
		}
	else
	{
			imageRect.x = 194;
	}

	App->renderer->Blit(graphics, 95, 32, &imageRect);


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
