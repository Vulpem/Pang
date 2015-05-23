#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL\include\SDL.h"

class ModuleTextures : public Module
{
public:
	ModuleTextures(Application* app, bool start_enabled = true);
	~ModuleTextures();

	bool Init();
	bool CleanUp();
	void Unload(SDL_Texture* texture);

	SDL_Texture* const Load(const char* path);
	SDL_Texture* const LoadSurface(SDL_Surface* surface);

public:
	p2List<SDL_Texture*> textures;
};