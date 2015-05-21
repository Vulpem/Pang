#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"

#include <string.h>
#include <sstream>

ModuleRender::ModuleRender(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	renderer = NULL;
}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	bool ret = true;
	Uint32 flags = 0;

	if(VSYNC == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
	}

	renderer = SDL_CreateRenderer(App->window->window, -1, flags);
	
	if(renderer == NULL)
	{
		LOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	TTF_Init();
	textColor = { 255, 255, 255 };
	font = LoadFont("Fonts/Pang.ttf", 8);
	fontInit = LoadFont("Fonts/Pang.ttf", 7);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRender::PreUpdate()
{
	SDL_RenderClear(renderer);
	return UPDATE_CONTINUE;
}

// Update: debug camera
update_status ModuleRender::Update()
{
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRender::PostUpdate()
{
	SDL_RenderPresent(renderer);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	TTF_CloseFont(font);
	TTF_CloseFont(fontInit);
	TTF_Quit();

	//Destroy window
	if(renderer != NULL)
	{
		SDL_DestroyRenderer(renderer);
	}

	return true;
}

// Blit to screen
bool ModuleRender::Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section, int xOffset, int yOffset, float speed)
{
	bool ret = true;
	SDL_Rect rect;
	rect.x = (x - xOffset) * SCREEN_SIZE;
	rect.y = (y - yOffset) * SCREEN_SIZE;

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= SCREEN_SIZE;
	rect.h *= SCREEN_SIZE;

	if(SDL_RenderCopy(renderer, texture, section, &rect) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if (use_camera)
	{
		rec.x = (int)(rect.x * SCREEN_SIZE);
		rec.y = (int)(rect.y * SCREEN_SIZE);
		rec.w *= SCREEN_SIZE;
		rec.h *= SCREEN_SIZE;
	}

	if (SDL_RenderFillRect(renderer, &rec) != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

TTF_Font* ModuleRender::LoadFont(char* file, int size) const
{
	TTF_Font* tmpfont;
	tmpfont = TTF_OpenFont(file, size);
	if (tmpfont == NULL)
	{
		LOG("Could not load font");
	}
	return tmpfont;
}

void ModuleRender::PrintNumbers(int num, SDL_Rect& rect, int x, int y) const
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
	}
	else
	{
		SDL_BlitSurface(tmpSurface, NULL, App->window->screen_surface, &rect);

		SDL_Texture* tmpTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
		if (tmpTexture)
		{
		App->renderer->Blit(tmpTexture, x, y, &rect);
		}

	}
	SDL_FreeSurface(tmpSurface);
	SDL_DestroyTexture(tmpTexture);
	*/
}


void ModuleRender::PrintText(char* text, SDL_Rect& rect, int x, int y, int size) const
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
	}

	else
	{
		tmpTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
		if (tmpTexture)
		{
			App->renderer->Blit(tmpTexture, x, y, &rect);
		}
	}

	SDL_FreeSurface(tmpSurface);
	SDL_DestroyTexture(tmpTexture);
	*/
}

void ModuleRender::drawText(char* string, int size, int y, int x, int R, int G, int B)
{
	
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, string, textColor);

	SDL_Rect textLoc = { x, y, 50, 50 };
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	Blit(textTexture, x, y, &textLoc);
	SDL_BlitSurface(textSurface, NULL, App->window->screen_surface, &textLoc);
	SDL_FreeSurface(textSurface);
	
}
