#include "Globals.h"
#include "Application.h"
#include "ModuleBackground.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleBackground::ModuleBackground(Application* app) : Module(app)
{

	graphics = NULL;
/*
	// ground
	ground.x = 8;
	ground.y = 391;
	ground.w = 896;
	ground.h = 72;
*/
	// Background / sky
	background.x = 8;
	background.y = 8;
	background.w = SCREEN_WIDTH;
	background.h = SCREEN_HEIGHT - 4*TILE;
	/*
	// flag animation
	flag.frames.PushBack({848, 208, 40, 40});
	flag.frames.PushBack({848, 256, 40, 40});
	flag.frames.PushBack({848, 304, 40, 40});
	flag.speed = 0.08f;
	*/
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
//	App->renderer->Blit(graphics, 560, 8, &(flag.GetCurrentFrame()), 0.75f); // flag animation

	
	return UPDATE_CONTINUE;
}