#pragma once

#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

class Ball
{
public:
	p2Point<int> position;
	p2Point<float> speed;

	int YBaseSpeed;
	int radius;
	int type;
	bool dead = false;

	SDL_Rect start_rect;
	int offset;

public:

	Ball(int x, int y, int _type, int direction);
	Ball(Ball* parent, int offsetDirection);
	bool Update();
};

class ModuleBalls : public Module
{
public:
	ModuleBalls(Application* app, bool start_enabled = true);
	~ModuleBalls();

	void AddBall(int position_x, int position_y, int _type, int _direction);

	p2List<Ball*> ballsList;
	SDL_Texture* ballsGraphics;
	SDL_Rect ballsRects[3][4];
	bool pauseBalls = false;

	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();


};
