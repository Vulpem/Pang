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
	
	LOG("Loading background assets");

	graphics = App->textures->Load("./Image_Sources/Backgrounds.png");

	App->player->Enable();
	App->balls->Enable();
	App->gun->Enable();
	
	App->balls->AddBall(150, 100, huge, 1);

	return true;
}

// Update: draw background
update_status ModuleScenePlay::Update()
{
	int b = App->balls->ballsList.count();
	if (App->balls->ballsList.count() == 0)
	{
		App->fade->FadeToBlack(this, App->backgroundIntro, 3.0f);
	}

	// Draw everything --------------------------------------
	App->renderer->Blit(graphics, 0, 0, &background, 0.75f);

	return UPDATE_CONTINUE;
}

bool ModuleScenePlay::CleanUp()
{
	App->textures->Unload(graphics);
	App->player->Disable();
	App->balls->Disable();
	App->gun->Disable();
	return true;
}