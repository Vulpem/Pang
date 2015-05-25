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
	for (int i = 0; i < UI_Transition_MAX; i++)
	{
		SDL_DestroyTexture(uiText[i]);
	}
	delete[]uiText;
}

bool ModuleSceneTransition::Init()
{
	uiText = new SDL_Texture*[UI_Transition_MAX];
	for (int i = 0; i < UI_Transition_MAX; i++)
	{
		uiText[i] == NULL;
	}

	uiText[UI_Transition_STAGE] = App->fonts->PrintText("STAGE", { 255, 255, 255 }, NULL);
	uiText[UI_Transition_TIMEBONUS] = App->fonts->PrintText("TIME BONUS", { 255, 255, 255 }, NULL);
	uiText[UI_Transition_PTS] = App->fonts->PrintText("PTS", { 255, 255, 255 }, NULL);
	uiText[UI_Transition_NEXTEXTEND] = App->fonts->PrintText("NEXT EXTEND", { 255, 255, 255 }, NULL);
	uiText[UI_Transition_PUSHBUTTON] = App->fonts->PrintText("PUSH BUTTON", { 255, 255, 255 }, NULL);

	rectText = new SDL_Rect[UI_Transition_MAX];

	rectText[UI_Transition_STAGE] = { 0, 0, 42.5, 8 };
	rectText[UI_Transition_TIMEBONUS] = { 0, 0, 85, 8 };
	rectText[UI_Transition_PTS] = { 0, 0, 25.5, 8 };
	rectText[UI_Transition_NEXTEXTEND] = { 0, 0, 93.5, 8 };
	rectText[UI_Transition_PUSHBUTTON] = { 0, 0, 93.5, 8 };

	return true;
}
bool ModuleSceneTransition::Start(int _nextLevel)
{
	App->audio->PlayMusic("./Sounds/TransitionScene.wav", 1);
	nextLevel = _nextLevel;
	imageRect = { 0, 0, 194, 96 };
	selectedRect = { 0, 15, 15, 15 };
	LOG("Scene Transition");
	bool ret = true;
	graphics = App->textures->Load("./Image_Sources/TransitionScene.png");
	graphics2 = App->textures->Load("./Image_Sources/IntroMap.png");
	selected = App->textures->Load("./Image_Sources/LevelSelected.png");

	uiText[UI_Transition_CHECKMARK] = App->fonts->PrintText("~", { 255, 167, 16 }, NULL);
	uiText[UI_Intro_STAGE] = App->fonts->PrintText("STAGE", { 255, 167, 16 }, NULL);

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

	//Change to next level after animation
	if (timeCounter >= 180 || (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP && timeCounter >= 50))
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
			if (timeCounter > 180)
			{
				timeCounter = 0;
				Disable();
				App->scenePlay->Enable(nextLevel * 3 - 2);
			}
			else
			{
				App->render->Blit(graphics2, 0, 0, NULL);
				if ((timeCounter / 2) % 2 == 0)
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
	//Level Number
	//Pending

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

void PrintMapInterface()
{

}