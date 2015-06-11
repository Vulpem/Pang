#include "Globals.h"
#include "Application.h"
#include "ModuleSceneTransition.h"

#define NUMLVLSPRINT 4
ModuleSceneTransition::ModuleSceneTransition(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	graphics = NULL;
	timeCounter = 0;
}

ModuleSceneTransition::~ModuleSceneTransition()
{
	for (int i = 0; i < UI_Transition_MAX; i++)
	{
		SDL_DestroyTexture(uiText[i]);
	}
	delete[]uiText;
	delete[] rectText;
}

bool ModuleSceneTransition::Init()
{
	uiText = new SDL_Texture*[UI_Transition_MAX];
	for (int i = 0; i < UI_Transition_MAX; i++)
	{
		uiText[i] = NULL;
	}

	uiText[UI_Transition_STAGE2] = App->fonts->PrintText("STAGE", { 255, 167, 16 }, NULL);
	uiText[UI_Transition_CHECKMARK] = App->fonts->PrintText("~", { 255, 167, 16 }, NULL);

	uiText[UI_Transition_STAGE] = App->fonts->PrintText("STAGE", { 255, 255, 255 }, NULL);
	uiText[UI_Transition_TIMEBONUS] = App->fonts->PrintText("TIME BONUS", { 255, 255, 255 }, NULL);
	uiText[UI_Transition_PTS] = App->fonts->PrintText("PTS.", { 255, 255, 255 }, NULL);
	uiText[UI_Transition_NEXTEXTEND] = App->fonts->PrintText("NEXT EXTEND", { 255, 255, 255 }, NULL);
	uiText[UI_Transition_PUSHBUTTON] = App->fonts->PrintText("PUSH BUTTON", { 255, 255, 255 }, NULL);

	uiText[UI_Transition_Text1_1] = App->fonts->PrintText("YOU'VE GOT A LONG WAY TO GO. ", { 255, 255, 255 }, NULL);
	uiText[UI_Transition_Text1_2] = App->fonts->PrintText("TAKE IT EASY. ", { 255, 255, 255 }, NULL);

	uiText[UI_Transition_Text2_1] = App->fonts->PrintText("YOU CAN BREAK SOME BLOCKS. ", { 255, 255, 255 }, NULL);
	uiText[UI_Transition_Text2_2] = App->fonts->PrintText("THEY ARE HIDDEN SOMEWHERE. ", { 255, 255, 255 }, NULL);

	uiText[UI_Transition_Text3_1] = App->fonts->PrintText("SORRY, CAN'T USE THE WIRE  ", { 255, 255, 255 }, NULL);
	uiText[UI_Transition_Text3_2] = App->fonts->PrintText("CONTINOUSLY. ONE AT A TIME  ", { 255, 255, 255 }, NULL);

	uiText[UI_Transition_Text4_1] = App->fonts->PrintText("YOU CAN SHOOT THE VULCAN MISSILE  ", { 255, 255, 255 }, NULL);
	uiText[UI_Transition_Text4_2] = App->fonts->PrintText("CONTINOUSLY BUT YOU CAN'T USE  ", { 255, 255, 255 }, NULL);
	uiText[UI_Transition_Text4_3] = App->fonts->PrintText("IT AGAIN  ", { 255, 255, 255 }, NULL);
	rectText = new SDL_Rect[UI_Transition_MAX];

	return true;
}
bool ModuleSceneTransition::Start(int _nextLevel)
{


	rectText[UI_Transition_STAGE2] = { 0, 0, 42.5, 8 };
	rectText[UI_Transition_CHECKMARK] = { 0, 0, 8, 8 };

	rectText[UI_Transition_STAGE] = { 0, 0, 42.5, 8 };
	rectText[UI_Transition_TIMEBONUS] = { 0, 0, 85, 8 };
	rectText[UI_Transition_PTS] = { 0, 0, 34, 8 };
	rectText[UI_Transition_NEXTEXTEND] = { 0, 0, 93.5, 8 };
	rectText[UI_Transition_PUSHBUTTON] = { 0, 0, 93.5, 8 };

	rectText[UI_Transition_Text1_1] = { 0, 0, 238, 8 };
	rectText[UI_Transition_Text1_2] = { 0, 0, 110.5, 8 };

	rectText[UI_Transition_Text2_1] = { 0, 0, 221, 8 };
	rectText[UI_Transition_Text2_2] = { 0, 0, 221, 8 };

	rectText[UI_Transition_Text3_1] = { 0, 0, 212.5, 8 };
	rectText[UI_Transition_Text3_2] = { 0, 0, 221, 8 };

	rectText[UI_Transition_Text4_1] = { 0, 0, 272, 8 };
	rectText[UI_Transition_Text4_2] = { 0, 0, 246.5, 8 };
	rectText[UI_Transition_Text4_2] = { 0, 0, 68, 8 };

	text1Lenght = rectText[nextLevel / 3 + 26].w + 1;
	text2Lenght = rectText[nextLevel / 3 + 26 + NUMLVLSPRINT].w + 1;
	text3Lenght = rectText[nextLevel / 3 + 26 + NUMLVLSPRINT * 2].w + 1;

	for (int i = UI_Transition_Text1_1; i < UI_Transition_MAX; i++)
	{
		rectText[i].w = 0;
	}

	App->audio->PlayMusic("./Sounds/TransitionScene.wav", 1);
	nextLevel = _nextLevel;
	imageRect = { 0, 0, 194, 96 };
	selectedRect = { 0, 15, 15, 15 };
	LOG("Scene Transition");
	bool ret = true;
	graphics = App->textures->Load("./Image_Sources/TransitionScene.png");
	graphics2 = App->textures->Load("./Image_Sources/TransitionMap.png");
	selected = App->textures->Load("./Image_Sources/LevelSelected.png");

	return ret;
}

bool ModuleSceneTransition::CleanUp()

{
	LOG("--Cleanup Transition scene")
	App->textures->Unload(graphics);
	App->textures->Unload(graphics2);
	App->textures->Unload(selected);

	return true;
}

// Update: draw background
update_status ModuleSceneTransition::Update()
{
	// Draw everything --------------------------------------

	timeCounter++;

	//Change to next level after animation
	if (timeCounter >= 180 || ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP || App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP) && timeCounter >= 50))
	{
		timeCounter = 0;
		if (nextLevel <= 10)
		{
			Disable();
			App->scenePlay->Enable(nextLevel);
		}
		else
		{
			Disable();
			App->sceneIntro->Enable();
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
		{
			PrintMapInterface();
			if (timeCounter > 180)
			{
				timeCounter = 0;
				Disable();
				App->scenePlay->Enable(nextLevel * 3 - 2);
			}
			else
			{
				App->render->Blit(graphics2, 0, 0, NULL);
				if ((timeCounter / 10) % 2 == 0)
					selectedRect.y = 0;
				else
					selectedRect.y = 15;
				App->render->Blit(selected, App->sceneIntro->SelectedPosition(true, nextLevel / 3 + 1), App->sceneIntro->SelectedPosition(false, nextLevel / 3 + 1), &selectedRect);
			}
		}


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
	//Push button
	if (timeCounter / 20 % 2 == 0)
	{
		App->render->Blit(uiText[UI_Transition_PUSHBUTTON], 280, 28 * TILE, &rectText[UI_Transition_PUSHBUTTON]);
	}

	//Level Number
	if ((nextLevel - 1) >= 10)
	{
		App->render->Blit(App->maps->textNumW[(nextLevel - 1) / 10], 150, 145, &App->maps->rectNum);
		App->render->Blit(App->maps->textNumW[(nextLevel - 1) % 10], 160, 145, &App->maps->rectNum);
	}
	else
		App->render->Blit(App->maps->textNumW[nextLevel - 1], 160, 145, &App->maps->rectNum);


	//Stage text
	App->render->Blit(uiText[UI_Transition_STAGE], 174, 145, &rectText[UI_Transition_STAGE]);
	App->render->Blit(uiText[UI_Transition_TIMEBONUS], 100, 165, &rectText[UI_Transition_TIMEBONUS]);
 
	//Time bonus points
	App->player->digitNumber = CountDigits(App->scenePlay->timeBonus);

	for (int i = 1; i <= App->player->digitNumber; i++)
	{
		App->player->rest = App->scenePlay->timeBonus % (int)(pow(10.0, i));
		App->player->div = pow(10.0, (i - 1));
		App->player->index = App->player->rest / App->player->div;

		App->render->Blit(App->maps->textNumW[App->player->index], 245 - (10 * (i - 1)), 165, &App->maps->rectNum);
	}


	App->render->Blit(uiText[UI_Transition_PTS], 264, 165, &rectText[UI_Transition_PTS]);

	if (timeCounter > 50)
	{
		App->render->Blit(uiText[UI_Transition_NEXTEXTEND], 100, 185, &rectText[UI_Transition_NEXTEXTEND]);
		App->render->Blit(uiText[UI_Transition_PTS], 264, 185, &rectText[UI_Transition_PTS]);

		App->render->Blit(App->maps->textNumW[1], 204, 185, &App->maps->rectNum);
		for (int i = 0; i < 4; i++)
		{
			App->render->Blit(App->maps->textNumW[(nextLevel - 1) / 10], 214 + 10 * i, 185, &App->maps->rectNum);
		}
	}
}

void ModuleSceneTransition::PrintMapInterface()
{
	//Level name
	if (nextLevel / 3 + 1 == 3)
	{
		if (uiText[UI_Transition_EMERALD] == NULL)
		{
			uiText[UI_Transition_EMERALD] = App->fonts->PrintText("EMERALD", { 255, 167, 16 }, NULL);
		}
		App->render->Blit(uiText[UI_Transition_EMERALD], 330 - App->player->rectText[UI_Transition_EMERALD].w / 2, 207, &App->player->rectText[UI_Transition_EMERALD]);

		if (uiText[UI_Transition_TEMPLE] == NULL)
		{
			uiText[UI_Transition_TEMPLE] = App->fonts->PrintText("TEMPLE", { 255, 167, 16 }, NULL);
		}
		App->render->Blit(uiText[UI_Transition_TEMPLE], 330 - App->player->rectText[UI_Transition_TEMPLE].w / 2, 217, &App->player->rectText[UI_Transition_TEMPLE]);
	}

	else if (nextLevel / 3 + 1 == 17)
	{
		if (uiText[UI_Transition_EASTER] == NULL)
		{
			uiText[UI_Transition_EASTER] = App->fonts->PrintText("EASTER", { 255, 255, 255 }, NULL);
		}
		App->render->Blit(uiText[UI_Transition_EASTER], 24 * TILE - rectText[UI_Transition_EASTER].w / 2, 26 * TILE, &rectText[UI_Transition_EASTER]);

		if (uiText[UI_Transition_ISLAND] == NULL)
		{
			uiText[UI_Transition_ISLAND] = App->fonts->PrintText("ISLAND", { 255, 255, 255 }, NULL);
		}
		App->render->Blit(uiText[UI_Transition_ISLAND], 24 * TILE - rectText[UI_Transition_ISLAND].w / 2, 27 * TILE, &rectText[UI_Transition_ISLAND]);
	}

	else
	{
		if (uiText[nextLevel] == NULL)
		{
			uiText[nextLevel] = App->fonts->PrintText(App->maps->GetLevelName(nextLevel), { 255, 167, 16 }, NULL);
		}
		App->render->Blit(uiText[nextLevel], 330 - App->player->rectText[nextLevel].w / 2, 207, &App->player->rectText[nextLevel]);
	}

	//Stage - Level info
	App->render->Blit(uiText[UI_Transition_STAGE2], 285, 230, &rectText[UI_Transition_STAGE2]);
	if ((nextLevel) >= 10)
	{
		App->render->Blit(App->maps->textNumY[(nextLevel) / 10], 335, 230, &App->maps->rectNum);
		App->render->Blit(App->maps->textNumY[(nextLevel) % 10], 345, 230, &App->maps->rectNum);
	}
	else
		App->render->Blit(App->maps->textNumY[nextLevel], 345, 230, &App->maps->rectNum);

	App->render->Blit(uiText[UI_Transition_CHECKMARK], 355, 230, &rectText[UI_Transition_CHECKMARK]);

	if (nextLevel + 3 >= 10)
	{
		App->render->Blit(App->maps->textNumY[(nextLevel + 3) / 10], 365, 230, &App->maps->rectNum);
		App->render->Blit(App->maps->textNumY[(nextLevel + 3) % 10], 375, 230, &App->maps->rectNum);
	}
	else
		App->render->Blit(App->maps->textNumY[nextLevel + 3], 365, 230, &App->maps->rectNum);

	PrintText();
}

void ModuleSceneTransition::PrintText()
{
	// UI_Transition_Text1_1
	if (nextLevel / 3 <= NUMLVLSPRINT)
	{
		if ((nextLevel / 3 + 1 == 5 && nextLevel / 3 + 1 == 6 && nextLevel / 3 + 1 == 8 && nextLevel / 3 + 1 == 10 &&
			 nextLevel / 3 + 1 == 12 && nextLevel / 3 + 1 == 13))
		{
			if (rectText[nextLevel / 3 + 26].w < text1Lenght)
			{
				rectText[nextLevel / 3 + 26].w = timeCounter * 10;
			}
			App->render->Blit(uiText[(nextLevel / 3) + 26], TILE, 210, &rectText[nextLevel / 3 + 26]);

			if (rectText[nextLevel / 3 + 26 + NUMLVLSPRINT].w < text2Lenght && (rectText[nextLevel / 3 + 26].w >= text1Lenght))
			{
				rectText[nextLevel / 3 + 26 + NUMLVLSPRINT].w = timeCounter * 10 - rectText[nextLevel / 3 + 26].w / 10;
			}
			App->render->Blit(uiText[nextLevel / 3 + 26 + NUMLVLSPRINT], TILE, 220, &rectText[nextLevel / 3 + 26 + NUMLVLSPRINT]);

			if (rectText[nextLevel / 3 + 26 + NUMLVLSPRINT * 2].w < text3Lenght && (rectText[nextLevel / 3 + 26 + NUMLVLSPRINT].w >= text2Lenght))
			{
				rectText[nextLevel / 3 + 26].w = timeCounter * 10 - (rectText[nextLevel / 3 + 26].w / 10) - (rectText[nextLevel / 3 + 26 + NUMLVLSPRINT].w / 10);
			}
			App->render->Blit(uiText[nextLevel / 3 + 26 + NUMLVLSPRINT * 2], TILE, 230, &rectText[nextLevel / 3 + 26 + NUMLVLSPRINT * 2]);
		}
		else
		{
			if (rectText[nextLevel / 3 + 26].w < text1Lenght - 10)
			{
				rectText[nextLevel / 3 + 26].w = timeCounter * 10;
			}
			App->render->Blit(uiText[nextLevel / 3 + 26], TILE, 215, &rectText[nextLevel / 3 + 26]);

			if (rectText[nextLevel / 3 + 26 + NUMLVLSPRINT].w < text2Lenght - 8.5 && (rectText[nextLevel / 3 + 26].w >= text1Lenght - 10))
			{
				rectText[nextLevel / 3 + 26 + NUMLVLSPRINT].w = (timeCounter - (rectText[nextLevel / 3 + 26].w / 10))* 10 ;
			}
			App->render->Blit(uiText[nextLevel / 3 + 26 + NUMLVLSPRINT], TILE, 225, &rectText[nextLevel / 3 + 26 + NUMLVLSPRINT]);
		}
	}

}