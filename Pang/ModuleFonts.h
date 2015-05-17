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

	TTF_Font* LoadFont(char* file, int size);
//	void PrintText(std::string text, SDL_Surface* surface);

public:
};