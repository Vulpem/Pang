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
	paused = false;
	selectedRect = { 0, 15, 15, 15 };
	timerRect = { 0, 0, 26, 38};
	interfaceRect = { 0, 0, 0, 10};
	nextLevel = 1;
	graphics = App->textures->Load("./Image_Sources/Pang_Title_NoCoin.png");
	graphics2 = App->textures->Load("./Image_Sources/Pang_Title_Coin.png");
	map = App->textures->Load("./Image_Sources/IntroMap.png");
	selected = App->textures->Load("./Image_Sources/LevelSelected.png");
	timer = App->textures->Load("./Image_Sources/MapTimer.png");
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
	if (paused)
	{
		timeCounter++;
		if (timeCounter > 30)
		{
			timeCounter = 0;
			App->backgroundPlay->Enable(nextLevel);
			Disable();
		}
		else
		{
		if ((timeCounter / 2) % 2 == 0)
			selectedRect.y = 0;
		else
			selectedRect.y = 15;
		App->renderer->Blit(map, 0, 0, NULL);
		App->renderer->Blit(selected, SelectedPosition(true), SelectedPosition(false), &selectedRect);
		}

	}
	else
	{
		if (!mapOn)
		{
			timeCounter++;
			if (timeCounter / 25 % 2 == 0)
				App->renderer->Blit(graphics, 0, 0, NULL);
			else
				App->renderer->Blit(graphics2, 0, 0, NULL);

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
			{
				timeCounter = 0;
				mapOn = true;
			}

		}
		if (mapOn)
		{
			//timeCounter++;
			App->renderer->Blit(map, 0, 0, NULL);
			//Drawing time counter
			timerRect.x = (timeCounter / 60) * 27;
			App->renderer->Blit(timer, 257, 31, &timerRect);
			
			//Moving through levels
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP && nextLevel < 10)
			{
				nextLevel++;
			}
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP && nextLevel > 1)
			{
				nextLevel--;
			}
			if ((App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || timeCounter >= 600) && nextLevel <= 10)
			{
				timeCounter = 0;
				paused = true;
			}
			App->renderer->Blit(selected, SelectedPosition(true), SelectedPosition(false), &selectedRect);
			App->fonts->PrintText("CHOSE THE CITY TO START.", interfaceText, interfaceRect, 8, 216, 6);
			App->fonts->PrintText("USE THE JOYSTICK TO CHOOSE.", interfaceText, interfaceRect, 8, 224, 6);
			App->fonts->PrintText("PRESS BUTTON TO FINALIZE CHOICE.", interfaceText, interfaceRect, 8, 232, 6);

		}
	}

	return UPDATE_CONTINUE;
}

int ModuleSceneIntro::SelectedPosition(bool x)
{
	switch (nextLevel)
	{
	case 1:
		if (x == true)
			return 341;
		else
			return 69;
		break;
	case 2:
		if (x == true)
			return 301;
		else
			return 69;
		break;
	case 3:
		if (x == true)
			return 285;
		else
			return 101;
		break;
	case 4:
		if (x == true)
			return 301;
		else
			return 101;
		break;
	case 5:
		if (x == true)
			return 329;
		else
			return 161;
		break;
	case 6:
		if (x == true)
			return 265;
		else
			return 97;
		break;
	case 7:
		if (x == true)
			return 205;
		else
			return 37;
		break;
	case 8:
		if (x == true)
			return 173;
		else
			return 45;
		break;
	case 9:
		if (x == true)
			return 149;
		else
			return 37;
	case 10:
		if (x == true)
			return 129;
		else
			return 65;
	default:
		break;
	}
}
