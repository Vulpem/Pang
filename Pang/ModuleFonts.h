#pragma once
#include "Module.h"
#include "Globals.h"
#include "List.h"
#include "SDL_TTF\include\SDL_ttf.h"


class ModuleFonts : public Module
{
public:

	p2List<TTF_Font*> fonts;
	TTF_Font* def;


public:
	ModuleFonts(Application* app, bool start_enabled = true);
	~ModuleFonts();


	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	TTF_Font* const LoadFont(char* file, int size);
	void PrintNumbers(int num, SDL_Rect& rect, int x, int y) const;
	void PrintText(char* text, SDL_Rect& rect, int x, int y, int size) const;

	SDL_Texture* Print(const char* text, SDL_Color color, TTF_Font* font, SDL_Rect& rect);
	SDL_Texture* Numbers(int numbers, SDL_Color color, TTF_Font* font, SDL_Rect& rect);

public:
};