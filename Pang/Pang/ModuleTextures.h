#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL\include\SDL.h"

class ModuleTextures : public Module
{
public:
	ModuleTextures(Application* app);
	~ModuleTextures();

	bool Init();
	bool CleanUp();

	SDL_Texture* const Load(const char* path);

public:
	p2List<SDL_Texture*> textures;
};