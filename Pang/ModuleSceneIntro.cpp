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
{
	
}

// Load assets
bool ModuleSceneIntro::Start()
{

  	LOG("--Initializing Intro");
 	bool ret = true;
	mapOn = false;
	App->maps->LoadMap(-1);
	paused = false;
	boolAnimation = true;
	App->player->waitingContinue = false;
	App->player2->waitingContinue = false;
	selectedRect = { 0, 15, 15, 15 };
	timerRect = { 0, 0, 26, 38};
	animationRect = { 0, 0, 384, 240 };
	nextLevel = 1;
	graphics = App->textures->Load("./Image_Sources/Pang_Title_NoCoin.png");
	graphics2 = App->textures->Load("./Image_Sources/Pang_Title_Coin.png");
	animation = App->textures->Load("./Image_Sources/Pang_Title_Balls.png");
	map = App->textures->Load("./Image_Sources/IntroMap.png");
	selected = App->textures->Load("./Image_Sources/LevelSelected.png");
	timer = App->textures->Load("./Image_Sources/MapTimer.png");
	App->audio->PlayMusic("./Sounds/Title_Screen.ogg", -1);

	uiText = new SDL_Texture*[UI_Intro_MAX];
	for (int i = 0; i < UI_Intro_MAX; i++)
	{
		uiText[i] = NULL;
	}

	uiText[UI_Intro_TEXT1] = App->fonts->PrintText("CHOSE THE CITY TO START. ", { 255, 255, 255 }, NULL);
	uiText[UI_Intro_TEXT2] = App->fonts->PrintText("USE JOYSTICK TO CHOOSE. ", { 255, 255, 255 }, NULL);
	uiText[UI_Intro_TEXT3] = App->fonts->PrintText("PRESS BUTTON TO FINALIZE CHOICE. ", { 255, 255, 255 }, NULL);

	uiText[UI_Intro_CHECKMARK] = App->fonts->PrintText("~", { 255, 167, 16 }, NULL);
	uiText[UI_Intro_STAGE] = App->fonts->PrintText("STAGE", { 255, 167, 16 }, NULL);

	rectText = new SDL_Rect[UI_Intro_MAX];

	rectText[UI_Intro_TEXT1] = { 0, 0, 0, 8 };
	rectText[UI_Intro_TEXT2] = { 0, 0, 0, 8 };
	rectText[UI_Intro_TEXT3] = { 0, 0, 0, 8 };
	rectText[UI_Intro_CHECKMARK] = { 0, 0, 8, 8 };
	rectText[UI_Intro_STAGE] = { 0, 0, 42.5, 8 };
	
	App->balls->Enable();
	App->particles->Enable();
	App->scenePlay->lives1 = 2;
	App->scenePlay->lives2 = 2;
	App->player->score = 0;
	App->player2->score = 0;

	return ret;
}

bool ModuleSceneIntro::CleanUp()
{
	LOG("--Cleanup Intro scene")
 	App->textures->Unload(graphics);
	for (int i = 0; i < UI_Intro_MAX; i++)
	{
		SDL_DestroyTexture(uiText[i]);
	}
	delete[] rectText;
	delete[]uiText;
	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	App->render->DrawQuad({ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, 0, 0, 0, 255);
	timeCounter++;

	// Draw everything --------------------------------------
	// First Animation
	if (boolAnimation)
	{
		if (timeCounter < 170)
		{
			App->render->Blit(animation, 0, 0, &animationRect);
		}
		else
			boolAnimation = false;
		//Create balls
		if (timeCounter == 15)
		{
			App->balls->AddBall(250, 0, 3, 0, -1);
			App->balls->ballsList.getFirst()->data->speed.x = -3.3f;
			App->balls->ballsList.getFirst()->data->speed.y = 18;
			App->balls->ballsList.getFirst()->data->YBaseSpeed = -6.3f;
			App->balls->ballsList.getFirst()->data->divisible = false;
		}
		if (timeCounter == 30)
		{
			App->balls->AddBall(25, 20, 3, 0, 1);
			App->balls->ballsList.getFirst()->next->data->speed.x = 2.3f;
			App->balls->ballsList.getFirst()->next->data->speed.y = 7;
			App->balls->ballsList.getFirst()->next->data->YBaseSpeed = -4.4f;
			App->balls->ballsList.getFirst()->next->data->divisible = false;
		}
		if (timeCounter == 45)
		{
			App->balls->AddBall(270, 20, 3, 0, -1);
			App->balls->ballsList.getFirst()->next->next->data->speed.x = -1.0f;
			App->balls->ballsList.getFirst()->next->next->data->speed.y = 7;
			App->balls->ballsList.getFirst()->next->next->data->YBaseSpeed = -5.0f;
			App->balls->ballsList.getFirst()->next->next->data->divisible = false;
		}
		if (timeCounter == 50)
		{
			App->balls->AddBall(35, 20, 3, 0, 1);
			App->balls->ballsList.getFirst()->next->next->next->data->speed.x = 3.7f;
			App->balls->ballsList.getFirst()->next->next->next->data->speed.y = 7;
			App->balls->ballsList.getFirst()->next->next->next->data->YBaseSpeed = -4.3f;
			App->balls->ballsList.getFirst()->next->next->next->data->divisible = false;
		}

		//Destroy balls
		if (timeCounter == 60)
		{
			App->balls->ballsList.getFirst()->data->dead = true;
			animationRect.x += 384;
		}
		if (timeCounter == 85)
		{
			App->balls->ballsList.getFirst()->data->dead = true;
			animationRect.x += 384;
		}
		if (timeCounter == 97)
		{
			App->balls->ballsList.getFirst()->data->dead = true;
			animationRect.x += 384;
		}
		if (timeCounter == 109)
		{
			App->balls->ballsList.getFirst()->data->dead = true;
			animationRect.x += 384;
		}
	}
	//End animation

	//Print animation when level selected
	if (paused)
	{
		if (timeCounter > 30)
		{
			timeCounter = 0;
			App->balls->Disable();
			Disable();
			App->player2->dead = false;
			App->scenePlay->player1Enabled = true;
			App->scenePlay->lives1 = App->scenePlay->lives2 = 2;
			App->scenePlay->Enable(nextLevel * 3 - 2);
		}
		else
		{
		if ((timeCounter / 2) % 2 == 0)
			selectedRect.y = 0;
		else
			selectedRect.y = 15;
		App->render->Blit(map, 0, 0, NULL);
		App->render->Blit(selected, SelectedPosition(true, nextLevel), SelectedPosition(false, nextLevel), &selectedRect);
		}
	}
	else
	{
		//Print intro image waiting for coin
		if (!mapOn && timeCounter > 170)
		{
			if (timeCounter / 25 % 2 == 0)
				App->render->Blit(graphics, 0, 0, NULL);
			else
				App->render->Blit(graphics2, 0, 0, NULL);

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP || App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP)
			{
				timeCounter = 0;
				mapOn = true;
			}

		}
		//Print map with stages
		if (mapOn)
		{
			if (timeCounter <= 600 && timeCounter % 60 == 0)
				App->audio->PlayMusic("./Sounds/Timer.wav", 1);
			App->render->Blit(map, 0, 0, NULL);
			//Drawing time counter
			timerRect.x = (timeCounter / 60) * 27;
			App->render->Blit(timer, 257, 31, &timerRect);

			//Moving through levels
			if ((App->input->GetKey(SDL_SCANCODE_A) == KEY_UP || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP) && nextLevel < 3)
			{
				nextLevel++;
			}
			if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_UP || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP) && nextLevel > 1)
			{
				nextLevel--;
			}
			if ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || timeCounter >= 600) && nextLevel <= 10)
			{
				timeCounter = 0;
				paused = true;
			}
			App->render->Blit(selected, SelectedPosition(true, nextLevel), SelectedPosition(false, nextLevel), &selectedRect);

			if (timeCounter < 33)
			{
				rectText[UI_Intro_TEXT1].w += 6;
			}
			if (timeCounter < 64 && timeCounter > 32)
			{
				rectText[UI_Intro_TEXT2].w += 6;
			}
			if (timeCounter < 96 && timeCounter > 63)
			{
				rectText[UI_Intro_TEXT3].w += 8;
			}
			App->render->Blit(uiText[UI_Intro_TEXT1], TILE, 210, &rectText[UI_Intro_TEXT1]);
			App->render->Blit(uiText[UI_Intro_TEXT2], TILE, 220, &rectText[UI_Intro_TEXT2]);
			App->render->Blit(uiText[UI_Intro_TEXT3], TILE, 230, &rectText[UI_Intro_TEXT3]);


			if (nextLevel == 3)
			{
				if (uiText[UI_Intro_EMERALD] == NULL)
				{
					uiText[UI_Intro_EMERALD] = App->fonts->PrintText("EMERALD", { 255, 167, 16 }, NULL);
				}
				App->render->Blit(uiText[UI_Intro_EMERALD], 330 - App->player->rectText[UI_Intro_EMERALD].w / 2, 207, &App->player->rectText[UI_Intro_EMERALD]);

				if (uiText[UI_Intro_TEMPLE] == NULL)
				{
					uiText[UI_Intro_TEMPLE] = App->fonts->PrintText("TEMPLE", { 255, 167, 16 }, NULL);
				}
				App->render->Blit(uiText[UI_Intro_TEMPLE], 330 - App->player->rectText[UI_Intro_TEMPLE].w / 2, 217, &App->player->rectText[UI_Intro_TEMPLE]);
			}

			else
			{
				if (uiText[nextLevel] == NULL)
				{
					uiText[nextLevel] = App->fonts->PrintText(App->maps->GetLevelName(nextLevel * 3), { 255, 167, 16 }, NULL);
				}
				App->render->Blit(uiText[nextLevel], 330 - App->player->rectText[nextLevel].w / 2, 207, &App->player->rectText[nextLevel]);
			}

			//Stage - Level info
			App->render->Blit(uiText[UI_Intro_STAGE], 285, 230, &rectText[UI_Intro_STAGE]);
			if ((nextLevel * 3 - 2) >= 10)
			{
				App->render->Blit(App->maps->textNumY[(nextLevel * 3 - 2) / 10 ], 335, 230, &App->maps->rectNum);
				App->render->Blit(App->maps->textNumY[(nextLevel * 3 - 2) % 10], 345, 230, &App->maps->rectNum);
			}
			else
				App->render->Blit(App->maps->textNumY[nextLevel * 3 - 2], 345, 230, &App->maps->rectNum);
			
			App->render->Blit(uiText[UI_Intro_CHECKMARK], 355, 230, &rectText[UI_Intro_CHECKMARK]);

			if (nextLevel * 3 >= 10)
			{
				App->render->Blit(App->maps->textNumY[(nextLevel * 3) / 10], 365, 230, &App->maps->rectNum);
				App->render->Blit(App->maps->textNumY[(nextLevel * 3) % 10], 375, 230, &App->maps->rectNum);
			}
			else
				App->render->Blit(App->maps->textNumY[nextLevel * 3], 365, 230, &App->maps->rectNum);
		}
	}

	return UPDATE_CONTINUE;
}

int ModuleSceneIntro::SelectedPosition(bool x, int _nextLevel)
{
	switch (_nextLevel)
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
