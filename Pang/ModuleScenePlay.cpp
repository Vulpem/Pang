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
{}

// Load assets
bool ModuleScenePlay::Start()
{
	interfaceRect.x = 0;
	interfaceRect.y = SCREEN_HEIGHT - (4 * TILE);
	interfaceRect.w = SCREEN_WIDTH;
	interfaceRect.h = 4 * TILE;
	LOG("Loading background assets");

	graphics = App->textures->Load("./Image_Sources/Backgrounds.png");
	App->audio->PlayMusic("./Sounds/1Mt_Fuji.mp3");

	App->maps->Enable();
	App->player->Enable();
	App->balls->Enable();
	App->gun->Enable();

	App->balls->AddBall(200, 50, little, blue, 1);
	
	LOG("M: Toggle undying mode\nN: Toggle debug mode\nB: Create a Ball\nV: Explode big balls\nC: Count Balls\nP: Pause\nR: Reset");
	return true;
}

// Update: draw background
update_status ModuleScenePlay::Update()
{
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
		App->fade->FadeToBlack(this, this);
	}
	if (App->balls->ballsList.count() == 0)
	{
		App->fade->FadeToBlack(App->backgroundPlay, App->backgroundIntro);
	}

	// Draw everything --------------------------------------
	App->renderer->Blit(graphics, 0, 0, &background, 0.75f);
	App->renderer->DrawQuad(interfaceRect, 0, 0, 0, 255);

	return UPDATE_CONTINUE;
}

bool ModuleScenePlay::CleanUp()
{
	App->textures->Unload(graphics);

	App->gun->Disable();
	App->balls->Disable();
	App->player->Disable();
	App->maps->Disable();

	return true;
}