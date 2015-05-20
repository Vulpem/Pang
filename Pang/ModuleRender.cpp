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

void ModuleRender::PrintNumbers(int num, SDL_Surface* surface, SDL_Rect& rect, int x, int y) const
{

	std::string string = std::to_string(num);
	rect.w = strlen(string.c_str()) * 9;
	x -= strlen(string.c_str()) * 9;


	surface = TTF_RenderText_Solid(font, string.c_str(), textColor);
	if (surface == NULL)
	{
		LOG("Could not load message");
	}
	else
	{
		int b = 0;
		if (SDL_Texture* tmpTexture = SDL_CreateTextureFromSurface(renderer, surface))
		{
			b++;
			App->renderer->Blit(tmpTexture, x, y, &rect);
			b++;
		}
	}
}

void ModuleRender::PrintText(char* text, SDL_Surface* surface, SDL_Rect& rect, int x, int y, int size) const
{
	if (size == 8)
	{
		rect.w = strlen(text) * 8.5;
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
	else
	{
		int c = 0;
		if (SDL_Texture* tmpTexture = SDL_CreateTextureFromSurface(renderer, surface))
		{
			c++;
			App->renderer->Blit(tmpTexture, x, y, &rect);
			c++;
		}
	}

}
