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
	graphics = App->textures->Load("./Image_Sources/End_Sky.png");
	font2 = App->fonts->LoadFont("./Fonts/Pang.ttf", 20);
	text1Text = App->fonts->PrintText("THANK YOU", { 255, 255, 255 }, NULL);
	text2Text = App->fonts->PrintText("FOR PLAYING", { 255, 255, 255 }, NULL);

	text1Rect = { 0, 0, 180, 8 };
	text2Rect = { 0, 0, 220, 8 };

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
//	App->render->DrawQuad({ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }, 0, 0, 0, 255);
	App->render->Blit(graphics, 0, 40, &imageRect);

	App->render->Blit(text2Text, 50, 250, &text1Rect);
	App->render->Blit(text2Text, 50, 280, &text1Rect);


	return UPDATE_CONTINUE;
}