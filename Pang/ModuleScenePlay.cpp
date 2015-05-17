#include "Globals.h"
#include "Application.h"
#include "ModuleScenePlay.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleScenePlay::ModuleScenePlay(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	graphics = NULL;

	background.x = 8;
	background.y = 8;
	background.w = SCREEN_WIDTH;
	background.h = SCREEN_HEIGHT - 4 * TILE;
}

ModuleScenePlay::~ModuleScenePlay()
{
}

// Load assets
bool ModuleScenePlay::Start(int level)
{
	player1Rect = { 0, 0, 0, 16 };
	currentLvl = level;

	interfaceRect.x = 0;
	interfaceRect.y = SCREEN_HEIGHT - (4 * TILE);
	interfaceRect.w = SCREEN_WIDTH;
	interfaceRect.h = 4 * TILE;
	LOG("Loading background assets");

	graphics = App->textures->Load("./Image_Sources/Backgrounds.png");
	livesGraphics = App->textures->Load("./Image_Sources/Player.png");
	sceneTransition = App->textures->Load("./Image_Sources/Transition_Scene.png");
	

	App->maps->Enable();
	App->player->Enable();
	App->gun->Enable();
	App->fonts->Enable();

	App->maps->LoadMap(currentLvl);

	livesRect.x = 154;
	livesRect.y = 44;
	livesRect.w = 16;
	livesRect.h = 16;

	lives = 3;
	
	LOG("M: Toggle undying mode\nN: Toggle debug mode\nB: Create a Ball\nV: Explode big balls\nC: Count Balls\nP: Pause\nR: Reset");
	return true;
}

// Update: draw background
update_status ModuleScenePlay::Update()
{
#pragma region DebugCode
	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
	{
		debugMode = !debugMode;
	}
	int x = rand() % (SCREEN_WIDTH - TILE * 6) + TILE * 3;
	int y = rand() % (SCREEN_HEIGHT- TILE * 10) + TILE * 3;
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		App->balls->AddBall(x, y, huge, rand()%3 ,1);
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
		App->maps->LoadMap(0);
	}
#pragma endregion

	if (App->balls->ballsList.count() == 0)
	{
		if (currentLvl >= 3)
		{
			App->backgroundIntro->Enable();
			Disable();
		}
		else
		{
			App->backgroundTransition->Enable(++currentLvl);
			Disable();
			/*
			int punt = App->player->punct;
			currentLvl++;
			App->maps->LoadMap(currentLvl);
			*/
		}
	}
	else
	{
		// Draw everything --------------------------------------
		App->renderer->Blit(graphics, 0, 0, &background, 0.75f);
		App->renderer->DrawQuad(interfaceRect, 0, 0, 0, 255);

		for (int n = 0; n < lives && n < 4; n++)
		{
			App->renderer->Blit(livesGraphics, (2 + n * 2) * TILE, 28 * TILE, &livesRect);
		}
		UpdateInterface();

	}

	return UPDATE_CONTINUE;
}

update_status ModuleScenePlay::PostUpdate()
{
	return UPDATE_CONTINUE;
}
bool ModuleScenePlay::CleanUp()
{
	App->textures->Unload(graphics);

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
