#include "Globals.h"
#include "Application.h"
#include "ModuleFonts.h"
#include "String.h"

#include <sstream>
#include <string>
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
	font = LoadFont("Fonts/Pang.ttf", 14);
	return true;
}

update_status ModuleFonts::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleFonts::Update()
{

	std::string caption_str = std::to_string(App->player->punctuation);
	message = TTF_RenderText_Solid(font, caption_str.c_str(), textColor);

	if (message == NULL)
	{
		LOG("Coult not load the message");
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(App->renderer->renderer, message); 
	SDL_Rect rect = { 0, 0, 60, 16 };

	App->renderer->Blit(texture, 250, 215, &rect);
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

TTF_Font* ModuleFonts::LoadFont(char* file, int size)
{
	TTF_Font* tmpfont;
	tmpfont = TTF_OpenFont(file, size);
	if (tmpfont == NULL)
	{
		LOG("Could not load font");
	}
	return tmpfont;
}

void ModuleFonts::PrintText(std::string text, SDL_Surface* surface)
{
	surface = TTF_RenderText_Solid(font, text.c_str(), textColor);
}
