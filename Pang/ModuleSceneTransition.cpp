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
		PrintStats();

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

void ModuleSceneTransition::PrintStats()
{
	//Level Number
	textText = App->fonts->PrintNumbers(nextLevel - 1, { 255, 255, 255 }, NULL, textRect);
	App->render->Blit(textText, 170  - textRect.w, 145, &textRect);

	//Stage text
	textText = App->fonts->PrintText("STAGE", { 255, 255, 255 }, NULL, textRect);
	App->render->Blit(textText, 174, 145, &textRect);

	textText = App->fonts->PrintText("TIME BONUS", { 255, 255, 255 }, NULL, textRect);
	App->render->Blit(textText, 100, 165, &textRect);

	textText = App->fonts->PrintNumbers(App->scenePlay->timeBonus, { 255, 255, 255 }, NULL, textRect);
	App->render->Blit(textText, 260 - textRect.w, 165, &textRect);

	textText = App->fonts->PrintText("PTS", { 255, 255, 255 }, NULL, textRect);
	App->render->Blit(textText, 264, 165, &textRect);

	if (timeCounter > 50)
	{
		textText = App->fonts->PrintText("NEXT EXTEND", { 255, 255, 255 }, NULL, textRect);
		App->render->Blit(textText, 100, 185, &textRect);

		textText = App->fonts->PrintText("PTS", { 255, 255, 255 }, NULL, textRect);
		App->render->Blit(textText, 264, 185, &textRect);
	}
	///////////////////
	//Print push button
	if (timeCounter / 20 % 2 == 0)
	{
		textText = App->fonts->PrintText("PUSH BUTTON", { 255, 255, 255 }, NULL, textRect);
		App->render->Blit(textText, 280, 220, &textRect);
	}
}