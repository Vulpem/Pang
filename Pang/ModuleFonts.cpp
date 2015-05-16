#include "Globals.h"
#include "Application.h"
#include "ModuleFonts.h"

#include "SDL_TTF\include\SDL_ttf.h"

ModuleFonts::ModuleFonts(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleFonts::~ModuleFonts()
{}

bool ModuleFonts::Init()
{
	TTF_Init();
	textColor = { 255, 255, 255 };
	font = loadFont("Fonts/Arial.ttf", 10);
	return true;
}

update_status ModuleFonts::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleFonts::Update()
{
	message = TTF_RenderText_Solid(font, "Points: ", textColor);

	if (message == NULL)
	{
		LOG("Coult not load the message");
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(App->renderer->renderer, message); 
	SDL_Rect rect = { 0, 0, 80, 16 };
	App->renderer->Blit(texture, 200, 215, &rect);
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleFonts::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleFonts::CleanUp()
{
	return true;
}

TTF_Font* ModuleFonts::loadFont(char* file, int size)
{
	TTF_Font* tmpfont;
	tmpfont = TTF_OpenFont("Fonts/Arial.ttf", size);
	if (tmpfont == NULL)
	{
		LOG("Could not load font");
	}
	return tmpfont;
}