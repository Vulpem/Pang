#pragma once

#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

enum BallTypes
{
	little,
	medium,
	big,
	huge
};

class Ball
{
public:
	p2Point<int> position;
	p2Point<int> speed;

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
	ModuleBalls(Application* app);
	~ModuleBalls();

	void AddBall(int position_x, int position_y, int _type, int _direction);

	p2List<Ball*> ballsList;
	bool Start();
	update_status Update();



};
