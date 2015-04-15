#pragma once
#include "Module.h"
#include "p2Point.h"
#include "List.h"
#include "Globals.h"


struct Bullet
{
	Animation anim;

	p2Point<int> start;
	p2Point<int> end;

	SDL_Rect start_rect;
	SDL_Rect end_rect;
};


class ModuleGun : public Module
{
public:
	ModuleGun(Application* app);
	~ModuleGun();

	bool Start();
	update_status Update();

	void AddBullet(p2Point<int>);

public:

private:
	p2List<Bullet*> active;


};