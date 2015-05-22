#include "Globals.h"
#include "Application.h"
#include "ModuleScenePlay.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleScenePlay::ModuleScenePlay(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleScenePlay::~ModuleScenePlay()
{
}

bool ModuleScenePlay::Init()
{
	backgroundGraphics = NULL;

	background.x = 8;
	background.y = 8;
	background.w = SCREEN_WIDTH;
	background.h = SCREEN_HEIGHT - 4 * TILE;

	timerImage = NULL;

	interfaceRect.x = 0;
	interfaceRect.y = SCREEN_HEIGHT - (4 * TILE);
	interfaceRect.w = SCREEN_WIDTH;
	interfaceRect.h = 4 * TILE;
	player1Rect = { 0, 0, 0, 16 };
	timerRect = { 0, 0, 71, 15 };

	readyRect = { 0, 0, 58, 19 };
	livesRect.x = 154;
	livesRect.y = 44;
	livesRect.w = 16;
	livesRect.h = 16;

	return true;
}

// Load assets
bool ModuleScenePlay::Start(int level)
{

	currentLvl = level;

	timer = 6180;
	startTimerevent = timer - 180;

	timerNumRect1 = { 0, 0, 13, 16 };
	timerNumRect2 = { 0, 0, 13, 16 };
	timerNumRect3 = { 0, 0, 13, 16 };

	LOG("Loading background assets");

	backgroundGraphics = App->textures->Load("./Image_Sources/Backgrounds.png");
	livesGraphics = App->textures->Load("./Image_Sources/Player.png");
	timerImage = App->textures->Load("./Image_Sources/Timer_Photo.png");
	ready = App->textures->Load("./Image_Sources/Ready.png");
	timerNum =  App->textures->Load("./Image_Sources/Timer_Numbers.png");

	if (timerImage == NULL)
	{
		LOG("Could not load timer");
		return false;
	}
	if (timerNum == NULL)
	{
		LOG("Could not load timerNum1");
		return false;
	}
	App->maps->Enable();
//	App->fonts->Enable();
	
	App->player->Enable();
	App->gun->Enable();

	App->particles->Enable();

	App->maps->LoadMap(currentLvl);

	App->player->pausePlayer = true;
	App->balls->pauseBalls = true;
	LOG("M: Toggle undying mode\nN: Toggle debug mode\nB: Create a Ball\nV: Explode big balls\nC: Count Balls\nP: Pause\nR: Reset");
	return true;
}

// Update: draw background
update_status ModuleScenePlay::Update()
{
	if (timer >= 0)
	{
		timer--;
	}
	if (timer <= 0 && App->player->dead == false)
	{
		App->player->Kill(0);
	}
	if (App->balls->ballsList.count() == 0)
	{
		App->sceneTransition->Enable(++currentLvl);
		Disable();
	}
	else
	{
		// Draw everything --------------------------------------

		//Setting timer images
		//////////////////////
		if (timer >= startTimerevent)
		{
			timerNumRect1.x = ((startTimerevent / FPS / 100) % 10) * 13;
			timerNumRect2.x = ((startTimerevent / FPS / 10) % 10) * 13;
			timerNumRect3.x = ((startTimerevent / FPS) % 10) * 13;
		}
		else
		{
			timerNumRect1.x = ((timer / FPS / 100) % 10) * 13;
			timerNumRect2.x = ((timer / FPS / 10) % 10) * 13;
			timerNumRect3.x = ((timer / FPS) % 10) * 13;
		}

		//////////////////////
		App->render->Blit(backgroundGraphics, 0, 0, &background, 0.75f);
		App->render->Blit(timerImage, 250, 9, &timerRect);
		App->render->Blit(timerNum, 330, 9, &timerNumRect1);
		App->render->Blit(timerNum, 345, 9, &timerNumRect2);
		App->render->Blit(timerNum, 360, 9, &timerNumRect3);
		App->render->DrawQuad(interfaceRect, 0, 0, 0, 255);

		for (int n = 0; n < lives && n < 4; n++)
		{
			App->render->Blit(livesGraphics, (2 + n * 2) * TILE, 28 * TILE, &livesRect);
		}
		UpdateInterface();
	}

	if ((timer > startTimerevent + 60 || timer / 5 % 2 == 0) && timer > startTimerevent)
		App->render->Blit(ready, 150, 100, &readyRect);
	if (timer == startTimerevent)
	{
		App->player->pausePlayer = false;
		App->balls->pauseBalls = false;
	}
	if (timer == startTimerevent * 0.4)
	{
		App->audio->PlayMusic("./Sounds/Getting_late.wav");
	}
	if (timer == startTimerevent * 0.15)
	{
		App->audio->PlayMusic("./Sounds/out_of_time.wav");
	}


#pragma region DebugCode
	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
	{
		debugMode = !debugMode;
	}
	App->player->undying = debugMode;
	if (debugMode == true)
	{
		int x = rand() % (SCREEN_WIDTH - TILE * 6) + TILE * 3;
		int y = rand() % (SCREEN_HEIGHT - TILE * 10) + TILE * 3;
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			App->balls->AddBall(x, y, huge, rand() % 3, 1);
		}
		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		{
			App->balls->AddBall(x, y, big, rand() % 3, 1);
		}
		if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		{
			App->balls->AddBall(x, y, medium, rand() % 3, 1);
		}
		if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
		{
			App->balls->AddBall(x, y, little, rand() % 3, 1);
		}
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		{
			currentLvl = 0;
			App->maps->LoadMap(0);
		}
		if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
		{
			timer = 300;
		}
		if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		{
			LOG("Pause\n");
			App->balls->pauseBalls = !App->balls->pauseBalls;
		}
		if (App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
		{
			LOG("Bomb \n");
			App->balls->Bomb();
		}
		if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
		{
			LOG("NBalls: %i \n", App->balls->ballsList.count());
		}
		if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
		{
			App->sceneTransition->Enable(++currentLvl);
			Disable();
		}
		if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		{
			App->boosts->AddBoost(rand() % 300 + 16, 101, none);
		}
		/*
		Player bounding box
		*/
		SDL_Rect boundingBox;
		boundingBox.x = App->player->position.x + 4;
		boundingBox.y = App->player->position.y + 5;
		boundingBox.w = 16;
		boundingBox.h = 27;
		App->render->DrawQuad(boundingBox, 0, 200, 0, 150);
		p2List_item<Ball*>* pointer = App->balls->ballsList.getFirst();
		while (pointer != NULL)
		{

			SDL_Rect myBallPos;
			myBallPos.h = myBallPos.w = 1;
			myBallPos.x = pointer->data->position.x;
			myBallPos.y = pointer->data->position.y;
			App->render->DrawQuad(myBallPos, 0, 255, 0, 255);
			pointer = pointer->next;
		}
	}
#pragma endregion

	return UPDATE_CONTINUE;
}

update_status ModuleScenePlay::PostUpdate()
{
	return UPDATE_CONTINUE;
}
bool ModuleScenePlay::CleanUp()
{
	App->textures->Unload(backgroundGraphics);

	App->particles->Disable();
	App->gun->Disable();
	App->player->Disable();
	App->maps->Disable();

	App->textures->Unload(livesGraphics);

	return true;
}

void ModuleScenePlay::UpdateInterface()
{

}

void ModuleScenePlay::Enable(int level)
{
		if (enabled == false)
		{
			enabled = true;
			Start(level);
		}
}
