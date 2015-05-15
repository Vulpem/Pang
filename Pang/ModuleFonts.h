#pragma once

#include "Module.h"
#include "Globals.h"

#include "SDL2_ttf-2.0.12\include\SDL_ttf.h"

class ModuleFonts : public Module
{
public:

	ModuleFonts(Application* app, bool start_enabled = true);
	~ModuleFonts();



	bool Start();
	update_status Update();
	bool CleanUp();

	SDL_Surface* screen;
	SDL_Surface* fontSurface;
	SDL_Color fColor;
	SDL_Rect fontRect;
	TTF_Font* font;

	void FontInit();

	void printF(char* c, int x, int y);

public:

};
