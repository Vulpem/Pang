#include "Globals.h"
#include "Application.h"
#include "ModuleFonts.h"
#include "String.h"

#include <sstream>
#include <string>
#include "SDL_TTF\include\SDL_ttf.h"
#include "SDL\include\SDL.h"

ModuleFonts::ModuleFonts(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleFonts::~ModuleFonts()
{}

bool ModuleFonts::Init()
{
	if (TTF_Init() != 0)
	{
		LOG("TTF_Init() Failed: ");
		return UPDATE_ERROR;
	}
	textColor = { 255, 255, 255 };
	font = LoadFont("Fonts/Pang.ttf", 8);
	if (font == NULL)
	{
		LOG("TTF_OpenFont() Failed");
		return UPDATE_ERROR;
	}
	fontInit = LoadFont("Fonts/Pang.ttf", 7);
	if (fontInit == NULL)
	{
		LOG("TTF_OpenFont() Failed");
		return UPDATE_ERROR;
	}


	textColor = { 255, 255, 255 };
	return true;
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
	TTF_CloseFont(font);
	TTF_CloseFont(fontInit);
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

void ModuleFonts::PrintNumbers(int num,  SDL_Rect& rect, int x, int y) const
{
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

}

void ModuleFonts::PrintText(char* text, SDL_Rect& rect, int x, int y, int size) const
{

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
}

