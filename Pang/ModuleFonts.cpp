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
	LOG("Init True Type Font library");
	bool ret = true;

	if (TTF_Init() == -1)
	{
		LOG("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		ret = false;
	}
	else
	{
		def = LoadFont("./Fonts/Pang.ttf", 8);
	}
	return ret;
}

bool ModuleFonts::Start()
{
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
	LOG("Freeing True Type fonts and library");
	
	p2List_item<TTF_Font*>* item;
	
	for (item = fonts.getFirst(); item != NULL; item = item->next)
	{
		TTF_CloseFont(item->data);
	}
	fonts.clear();
	
	TTF_Quit();
	return true;
}

TTF_Font* const ModuleFonts::LoadFont(char* file, int size)
{
	TTF_Font* font = TTF_OpenFont(file, size);

	if (font == NULL)
	{
		LOG("Could not load font", TTF_GetError());
	}
	else
	{
		LOG("Successfully loaded font %s size %d", file, size);
		fonts.add(font);
	}
	return font;
}

void ModuleFonts::PrintNumbers(int num,  SDL_Rect& rect, int x, int y) const
{
	/*
	SDL_Surface* tmpSurface = NULL;
	SDL_Texture* tmpTexture = NULL;

	std::string string = std::to_string(num);
	rect.w = strlen(string.c_str()) * 9;
	x -= strlen(string.c_str()) * 9;


	tmpSurface = TTF_RenderText_Solid(font, string.c_str(), textColor);
	if (tmpSurface == NULL)
	{
		LOG("Could not load message");
		assert(false);
	}
	else
	{
		SDL_Texture* tmpTexture = SDL_CreateTextureFromSurface(App->render->renderer, tmpSurface);
		if (tmpTexture == NULL)
		{
			LOG("Could not generate texture");
			assert(false);
		}
		else
		{
			App->render->Blit(tmpTexture, x, y, &rect);
		}

	}
	if (tmpSurface)
	{
		SDL_FreeSurface(tmpSurface);
	}
	if (tmpTexture)
	{
		SDL_DestroyTexture(tmpTexture);
	}
	*/
}

void ModuleFonts::PrintText(char* text, SDL_Rect& rect, int x, int y, int size) const
{
	/*
	SDL_Surface* tmpSurface = NULL;
	SDL_Texture* tmpTexture = NULL;
	if (size == 8)
	{
		rect.w = strlen(text) * 8.5;
		tmpSurface = TTF_RenderText_Solid(font, text, textColor);
	}

	else if (size == 6)
	{
		rect.w = strlen(text) * 7;
		tmpSurface = TTF_RenderText_Solid(fontInit, text, textColor);
	}

	if (tmpSurface == NULL)
	{
		LOG("Could not load message");
		assert(false);
	}
	else
	{
		tmpTexture = SDL_CreateTextureFromSurface(App->render->renderer, tmpSurface);
		if (tmpSurface)
		{
			SDL_FreeSurface(tmpSurface);
		}
		if (tmpTexture == NULL)
		{
			LOG("Could not generate texture");
			assert(false);
		}
		else
		{
			App->render->Blit(tmpTexture, x, y, &rect);
		}
	}

	if (tmpTexture)
	{
		SDL_DestroyTexture(tmpTexture);
	}
	*/
}

SDL_Texture* ModuleFonts::Print(const char* text, SDL_Color color, TTF_Font* font, SDL_Rect& rect) 
{
	SDL_Texture* ret = NULL;
	SDL_Surface* surface = TTF_RenderText_Solid((font) ? font : def, text, color);
	rect.w = strlen(text) * 8.5;
	if (surface == NULL)
	{
		LOG("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		ret = App->textures->LoadSurface(surface);
		SDL_FreeSurface(surface);
	}

	return ret;
}

SDL_Texture* ModuleFonts::Numbers(int numbers, SDL_Color color, TTF_Font* font, SDL_Rect& rect)
{
	std::string string = std::to_string(numbers);
	rect.w = strlen(string.c_str()) * 9;
	
	SDL_Texture* ret = NULL;
	SDL_Surface* surface = TTF_RenderText_Solid((font) ? font : def, string.c_str(), color);
	if (surface == NULL)
	{
		LOG("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		ret = App->textures->LoadSurface(surface);
		SDL_FreeSurface(surface);
	}
	return ret;
}
