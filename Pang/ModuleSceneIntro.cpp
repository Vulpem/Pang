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
	textRect = { 0, 0, 0, 10};
	nextLevel = 1;
	textFont = App->fonts->LoadFont("./Fonts/Pang.ttf", 10);
	graphics = App->textures->Load("./Image_Sources/Pang_Title_NoCoin.png");
	graphics2 = App->textures->Load("./Image_Sources/Pang_Title_Coin.png");
	map = App->textures->Load("./Image_Sources/IntroMap.png");
	selected = App->textures->Load("./Image_Sources/LevelSelected.png");
	timer = App->textures->Load("./Image_Sources/MapTimer.png");
	App->audio->PlayMusic("./Sounds/Title_Screen.ogg");

	uiText = new SDL_Texture*[INTRO_UI_MAX];
	for (int i = 0; i < INTRO_UI_MAX; i++)
	{
		uiText[i] = NULL;
	}

	uiText[UI_TEXT1] = App->fonts->PrintText("CHOSE THE CITY TO START.", { 255, 255, 255 }, NULL, textRect);
	uiText[UI_TEXT2] = App->fonts->PrintText("USE JOYSTICK TO CHOOSE.", { 255, 255, 255 }, NULL, textRect);
	uiText[UI_TEXT3] = App->fonts->PrintText("PRESS BUTTON TO FINALIZE CHOICE.", { 255, 255, 255 }, NULL, textRect);

	rectText = new SDL_Rect[INTRO_UI_MAX];

	rectText[UI_TEXT1] = { 0, 0, 204, 8 };
	rectText[UI_TEXT2] = { 0, 0, 195.5, 8 };
	rectText[UI_TEXT3] = { 0, 0, 272, 8 };
	
	App->scenePlay->lives = 3;

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
			App->scenePlay->Enable(nextLevel * 3 - 2);
			Disable();
		}
		else
		{
		if ((timeCounter / 2) % 2 == 0)
			selectedRect.y = 0;
		else
			selectedRect.y = 15;
		App->render->Blit(map, 0, 0, NULL);
		App->render->Blit(selected, SelectedPosition(true), SelectedPosition(false), &selectedRect);
		}

	}
	else
	{
		if (!mapOn)
		{
			timeCounter++;
			if (timeCounter / 25 % 2 == 0)
				App->render->Blit(graphics, 0, 0, NULL);
			else
				App->render->Blit(graphics2, 0, 0, NULL);

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
			{
				timeCounter = 0;
				mapOn = true;
			}

		}
		if (mapOn)
		{
			timeCounter++;
			App->render->Blit(map, 0, 0, NULL);
			//Drawing time counter
			timerRect.x = (timeCounter / 60) * 27;
			App->render->Blit(timer, 257, 31, &timerRect);
			
			//Moving through levels
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP && nextLevel < 10)
			{
				nextLevel++;
			}
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP && nextLevel > 1)
			{
				nextLevel--;
			}
			if ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || timeCounter >= 600) && nextLevel <= 10)
			{
				timeCounter = 0;
				paused = true;
			}
			App->render->Blit(selected, SelectedPosition(true), SelectedPosition(false), &selectedRect);

			App->render->Blit(uiText[UI_TEXT1], TILE, 210, &rectText[UI_TEXT1]);
			App->render->Blit(uiText[UI_TEXT2], TILE, 220, &rectText[UI_TEXT2]);
			App->render->Blit(uiText[UI_TEXT3], TILE, 230, &rectText[UI_TEXT3]);


			if (nextLevel == 3)
			{
				textText = App->fonts->PrintText("EMERALD", { 255, 167, 16 }, NULL, textRect);
				App->render->Blit(textText, 330 - textRect.w / 2, 207, &textRect);

				textText = App->fonts->PrintText("TEMPLE", { 255, 167, 16 }, NULL, textRect);
				App->render->Blit(textText, 330 - textRect.w / 2, 217, &textRect);
			}
			else if (nextLevel == 17)
			{
				textText = App->fonts->PrintText("EASTER", { 255, 167, 16 }, NULL, textRect);
				App->render->Blit(textText, 330 - textRect.w / 2, 207, &textRect);

				textText = App->fonts->PrintText("ISLAND", { 255, 167, 16 }, NULL, textRect);
				App->render->Blit(textText, 330 - textRect.w / 2, 217, &textRect);
			}
			else
			{
				textText = App->fonts->PrintText((App->maps->GetLevelName(nextLevel * 3)), { 255, 167, 16 }, NULL, textRect);
				App->render->Blit(textText, 330 - textRect.w / 2, 207, &textRect);
			}

			textText = App->fonts->PrintNumbers(nextLevel * 3 - 2, { 255, 167, 16 }, NULL, textRect);
			App->render->Blit(textText, 350 - textRect.w, 230, &textRect);

			textText = App->fonts->PrintText("~", { 255, 167, 16 }, NULL, textRect);
			App->render->Blit(textText, 357 - textRect.w / 2, 230, &textRect);

			textText = App->fonts->PrintNumbers(nextLevel * 3, { 255, 167, 16 }, NULL, textRect);
			App->render->Blit(textText, 365, 230, &textRect);

			textText = App->fonts->PrintText("STAGE", { 255, 167, 16 }, NULL, textRect);
			App->render->Blit(textText, 285, 230, &textRect);
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
