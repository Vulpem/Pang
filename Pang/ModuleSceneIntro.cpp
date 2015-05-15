#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	graphics = NULL;
	graphics2 = NULL;
	timeCounter = 0;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{

	LOG("--Initializing Intro");
	bool ret = true;

	graphics = App->textures->Load("./Image_Sources/Pang_Title_NoCoin.png");
	graphics2 = App->textures->Load("./Image_Sources/Pang_Title_Coin.png");
	App->audio->PlayMusic("./Sounds/Title_Screen.ogg");

	return ret;
}

bool ModuleSceneIntro::CleanUp()
{
	LOG("--Cleanup Intro scene")
 	App->textures->Unload(graphics);

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	// Draw everything --------------------------------------
	timeCounter++;
	if (timeCounter / 25 % 2 == 0)
		App->renderer->Blit(graphics, 0, 0, NULL);
	else
		App->renderer->Blit(graphics2, 0, 0, NULL);

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
	{
		App->fade->FadeToBlack(this, App->backgroundPlay, 3.0f);
	}

	return UPDATE_CONTINUE;
}

