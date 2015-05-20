#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL_TTF\include\SDL_ttf.h"


class ModuleFonts : public Module
{
public:

	TTF_Font* font;
	TTF_Font* fontInit;

	SDL_Color textColor;

public:
	ModuleFonts(Application* app, bool start_enabled = true);
	~ModuleFonts();


	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	TTF_Font* LoadFont(char* file, int size) const;
	void PrintNumbers(int num, SDL_Surface* surface, SDL_Rect& rect, int x, int y) const;
	void PrintText(char* text, SDL_Surface* surface, SDL_Rect& rect, int x, int y, int size) const;

public:
};