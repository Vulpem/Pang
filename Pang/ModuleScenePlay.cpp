#include "Globals.h"
#include "Application.h"
#include "ModuleScenePlay.h"
#include <iostream>

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

	App->maps->Enable();
	App->player->Enable();
	App->balls->Enable();
	App->gun->Enable();
	
	App->balls->AddBall(50, 50, huge, 1);
	/*App->balls->AddBall(100, 50, big, 1);
	App->balls->AddBall(300, 50, medium, 1);
	App->balls->AddBall(200, 50, little, 1);*/
	
	std::cout << "M: Toggle undying mode" << std::endl
		<< "N: Toggle debug mode" << std::endl
		<< "B: Create a Ball" << std::endl
		<< "V: Explode big balls" << std::endl
		<< "C: Count Balls" << std::endl
		<< "P: Pause" << std::endl
		<< "R: Reset" << std::endl
		<< std::endl;
	return true;
}

// Update: draw background
update_status ModuleScenePlay::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
	{
		debugMode = !debugMode;
	}
	if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
	{
		App->balls->AddBall(200, 50, huge, 1);
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