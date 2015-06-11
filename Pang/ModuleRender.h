#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL_TTF\include\SDL_ttf.h"
#include <string.h>

class ModuleRender : public Module
{
private:
	TTF_Font* font;
	TTF_Font* fontInit;
	TTF_Font* fontEnd;
	SDL_Color textColor;

public:
	ModuleRender(Application* app, bool start_enabled = true);
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	bool Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section, int xOffset = 0, int yOffset = 0, float speed = 1.0f);
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera = true);

	TTF_Font* LoadFont(char* file, int size) const;
	void PrintNumbers(int num, SDL_Rect& rect, int x, int y) const;
	void PrintText(char* text, SDL_Rect& rect, int x, int y, int size) const;
	void drawText(char* string, int size, int y, int x, int R, int G, int B);
public:
	SDL_Renderer* renderer;
};