#include "Globals.h"
#include "Application.h"
#include "ModuleBackground.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleBackground::ModuleBackground(Application* app) : Module(app)
{
	graphics = NULL;

	background.x = 8;
	background.y = 8;
	background.w = SCREEN_WIDTH;
	background.h = SCREEN_HEIGHT - 4 * TILE;
}

ModuleBackground::~ModuleBackground()
{}

// Load assets
bool ModuleBackground::Start()
{
	App->balls->AddBall(150, 100, 4, 1);
	LOG("Loading background assets");
	bool ret = true;
	graphics = App->textures->Load("./Image_Sources/Backgrounds.png");
	return ret;
}

// Update: draw background
update_status ModuleBackground::Update()
{

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		App->balls->ballsList.getFirst()->data->dead = true;
	}
	// Draw everything --------------------------------------
	App->renderer->Blit(graphics, 0, 0, &background, 0.75f);

	return UPDATE_CONTINUE;
}