#include "Globals.h"
#include "Application.h"
#include "ModuleSceneEnd.h"

ModuleSceneEnd::ModuleSceneEnd(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleSceneEnd::~ModuleSceneEnd()
{

}

// Load assets
bool ModuleSceneEnd::Start()
{

	LOG("--Initializing End scene");
	App->audio->PlayMusic("./Sounds/The_end.wav", 1);
	graphics = App->textures->Load("./Image_Sources/End_Sky.png");
	font2 = App->fonts->LoadFont("./Fonts/Pang.ttf", 20);
	text1Text = App->fonts->PrintText("THANK YOU", { 255, 255, 255 }, font2);
	text2Text = App->fonts->PrintText("FOR PLAYING", { 255, 255, 255 }, font2);

	text1Rect = { 0, 0, 200, 20 };
	text2Rect = { 0, 0, 240, 20 };

	imageRect = { 0, 0, 384, 86 };
	bool ret = true;
	return ret;
}

bool ModuleSceneEnd::CleanUp()
{
	App->textures->Unload(graphics);
	LOG("--Cleanup Intro scene")
	return true;
}

// Update: draw background
update_status ModuleSceneEnd::Update()
{
	App->render->DrawQuad({ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }, 0, 0, 0, 255);
	App->render->Blit(graphics, 0, 40, &imageRect);

	App->render->Blit(text1Text, 50, 170, &text1Rect);
	App->render->Blit(text2Text, 50, 190, &text2Rect);

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP || App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP)
	{
		Disable();
		App->sceneIntro->Enable();
	}
	return UPDATE_CONTINUE;
}