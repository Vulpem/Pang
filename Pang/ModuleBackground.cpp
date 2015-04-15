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
	background.h = SCREEN_HEIGHT - 4*TILE;
}

ModuleBackground::~ModuleBackground()
{}

// Load assets
bool ModuleBackground::Start()
{
	LOG("Loading background assets");
	bool ret = true;
	graphics = App->textures->Load("./Image_Sources/Backgrounds.png");
	return ret;
}

// Update: draw background
update_status ModuleBackground::Update()
{
	// Draw everything --------------------------------------
	App->renderer->Blit(graphics, 0, 0, &background, 0.75f); // sea and sky

	return UPDATE_CONTINUE;
}