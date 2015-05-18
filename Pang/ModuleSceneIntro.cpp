#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	graphics = NULL;
	graphics2 = NULL;
	timeCounter = 0;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{

	LOG("--Initializing Intro");
	bool ret = true;
	mapOn = false;
	selectedRect = { 0, 0, 15, 15 };
	nextLevel = 1;
	graphics = App->textures->Load("./Image_Sources/Pang_Title_NoCoin.png");
	graphics2 = App->textures->Load("./Image_Sources/Pang_Title_Coin.png");
	map = App->textures->Load("./Image_Sources/IntroMap.png");
	selected = App->textures->Load("./Image_Sources/LevelSelected.png");
	App->audio->PlayMusic("./Sounds/Title_Screen.ogg");
	App->backgroundPlay->lives = 3;

	return ret;
}

bool ModuleSceneIntro::CleanUp()
{
	LOG("--Cleanup Intro scene")
 	App->textures->Unload(graphics);

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	// Draw everything --------------------------------------
	if (!mapOn)
	{
		timeCounter++;
		if (timeCounter / 25 % 2 == 0)
			App->renderer->Blit(graphics, 0, 0, NULL);
		else
			App->renderer->Blit(graphics2, 0, 0, NULL);

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
			mapOn = true;
	}
	if (mapOn)
	{
		App->renderer->Blit(map, 0, 0, NULL);
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP && nextLevel < 6)
		{
			nextLevel++;
		}
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP && nextLevel > 1)
		{
			nextLevel--;
		}
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && nextLevel <= 3)
		{
			timeCounter = 0;
			App->backgroundPlay->Enable(nextLevel);
			Disable();
		}
		switch (nextLevel)
		{
		case 1:
			App->renderer->Blit(selected, 341, 69, &selectedRect);
			break;
		case 2:
			App->renderer->Blit(selected, 301, 69, &selectedRect);
			break;
		case 3:
			App->renderer->Blit(selected, 285, 101, &selectedRect);
			break;
		case 4:
			App->renderer->Blit(selected, 301, 101, &selectedRect);
			break;
		case 5:
			App->renderer->Blit(selected, 329, 161, &selectedRect);
			break;
		case 6:
			App->renderer->Blit(selected, 265, 97, &selectedRect);
			break;

		}
	}

	return UPDATE_CONTINUE;
}

