#pragma once

#include "Module.h"
#include "Globals.h"

class ModuleMaps : public Module
{
public:
	SDL_Rect tile;
	static int map[26][48];
	const static int lvl[16][26][48];
	

	ModuleMaps(Application* app, bool start_enabled = true);
	~ModuleMaps();

	bool Init();
	bool Start();
	update_status Update();
	bool CleanUp();

	void LoadMap(int);

	void DrawGlassBrick(int h, int w);
	void DrawBrick(int h, int w);

	char* GetLevelName(int level);

public:

	SDL_Texture* ladderGraphics;
	SDL_Texture* bricksGraphics;
	SDL_Rect SolidBrickSection;
	Animation* brick_Animation[5];
	
};
