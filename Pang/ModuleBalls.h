#pragma once

#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

class Ball
{
public:
	p2Point<float> position;
	p2Point<float> speed;

	float YBaseSpeed;
	int radius;
	int type;
	int color;
	bool dead = false;

	int offset;

public:
	Ball(int x, int y, int _type, int _color, int direction);
	Ball(Ball* parent, int offsetDirection);
	void Update(bool pause);
private:
	void CreateBall(int direction, float ySpeedChange = 1.0f);
	
};

class ModuleBalls : public Module
{
public:

	ModuleBalls(Application* app, bool start_enabled = true);
	~ModuleBalls();

	p2List<Ball*> ballsList;
	SDL_Texture* ballsGraphics;
	SDL_Rect ballsRects[3][4];
	bool pauseBalls = false;
	int pauseCounter;
	bool pauseBoost;
	int bombCounter;
	bool bombBoost;

	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();

	void AddBall(int position_x, int position_y, int _type, int _color, int _direction);
	void CheckBricksColision(p2List_item<Ball*>*);
	bool CheckColision(int tileX, int tileY, Ball* myBall);
	void CheckBoosts();
	void PauseBoost();
	void BombBoost();
	void Bomb();

};
