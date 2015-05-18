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

	bool Update(Application* app);
};


class ModuleGun : public Module
{
public:

	ModuleGun(Application* app, bool start_enabled = true);
	~ModuleGun();

	bool shootAvailable = true;
	int maxShots;
	int bulletWidth;

	SDL_Texture* graphics;

	Animation* current_animation;
	Animation hook;

public:
	bool Init();
	bool Start();
	update_status Update();
	bool CleanUp();

	void AddBullet(p2Point<int>);

	void Shoot(p2Point<int>);

public:
	p2List<Bullet*> activeBullet;


};