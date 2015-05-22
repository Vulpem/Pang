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
	App->audio->PlayMusic("./Sounds/TransitionScene.wav");
	textRect = { 0, 0, 0, 9 };
	nextLevel = _nextLevel;
	imageRect = { 0, 0, 194, 96 };
	LOG("Scene Transition");
	bool ret = true;
	graphics = App->textures->Load("./Image_Sources/TransitionScene.png");
	graphics2 = App->textures->Load("./Image_Sources/IntroMap.png");

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
	if (timeCounter >= 180 || (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP && timeCounter >= 50))
	{
		timeCounter = 0;
		if (nextLevel <= 10)
		{
			App->scenePlay->Enable(nextLevel);
			Disable();
		}
		else
		{
			App->sceneIntro->Enable();
			Disable();
		}
	}
	App->render->DrawQuad({ 0, 0, 384, 240 }, 8, 8, 8, 255);
	
	//Print level stats if its not end of stage
	if ((nextLevel - 1) % 3 != 0)
	{
		App->fonts->PrintNumbers(nextLevel - 1, textRect, 170, 145);
		App->fonts->PrintText("STAGE", textRect, 170 + 4, 145, 8);

		App->fonts->PrintText("TIME BONUS", textRect, 100, 165, 8);
		App->fonts->PrintText("PTS", textRect, 260 + 4, 165, 8);
		if (timeCounter > 50)
		{
			App->fonts->PrintText("NEXT EXTEND", textRect, 100, 185, 8);
			App->fonts->PrintText("PTS", textRect, 260 + 4, 185, 8);
		}
		///////////////////
		//Print push button
		if (timeCounter / 20 % 2 == 0)
		{
			App->fonts->PrintText("PUSH BUTTON", textRect, 280, 220, 8);
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

		App->render->Blit(graphics, 95, 32, &imageRect);
	}
	//If it's end of stage
	else
		App->render->Blit(graphics2, 0, 0, NULL);

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
