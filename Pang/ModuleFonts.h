#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL_TTF\include\SDL_ttf.h"

class ModuleFonts : public Module
{
public:

	SDL_Surface* message;


	TTF_Font* font;

	SDL_Color textColor;

public:
	ModuleFonts(Application* app, bool start_enabled = true);
	~ModuleFonts();


	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	TTF_Font* loadFont(char* file, int size);
public:
};