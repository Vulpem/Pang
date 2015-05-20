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
	font = LoadFont("Fonts/Pang.ttf", 8);
	fontInit = LoadFont("Fonts/Pang.ttf", 7);
	return true;
}

update_status ModuleFonts::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleFonts::Update()
{
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleFonts::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleFonts::CleanUp()
{
	TTF_Quit();
	return true;
}

TTF_Font* ModuleFonts::LoadFont(char* file, int size) const
{
	TTF_Font* tmpfont;
	tmpfont = TTF_OpenFont(file, size);
	if (tmpfont == NULL)
	{
		LOG("Could not load font");
	}
	return tmpfont;
}

void ModuleFonts::PrintNumbers(int num, SDL_Surface* surface, SDL_Rect& rect, int x, int y) const
{
	
	std::string string = std::to_string(num);
	rect.w = strlen(string.c_str()) * 9;
	x -= strlen(string.c_str()) * 9;


	surface = TTF_RenderText_Solid(font, string.c_str(), textColor);
	if (surface == NULL)
	{
		LOG("Could not load message");
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(App->renderer->renderer, surface);
	App->renderer->Blit(texture, x, y, &rect);
	
}

void ModuleFonts::PrintText(char* text, SDL_Surface* surface, SDL_Rect& rect, int x, int y, int size) const
{
	/*
	if (size == 8)
	{
		rect.w = strlen(text) * 8;
		surface = TTF_RenderText_Solid(font, text, textColor);
	}

	else if (size == 6)
	{
		rect.w = strlen(text) * 7;
		surface = TTF_RenderText_Solid(fontInit, text, textColor);
	}

	if (surface == NULL)
	{
		LOG("Could not load message");
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(App->renderer->renderer, surface);
	App->renderer->Blit(texture, x, y, &rect);
	*/

}

