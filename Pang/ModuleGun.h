#pragma once
#include "Module.h"
#include "p2Point.h"
#include "List.h"
#include "Globals.h"

enum GunType
{
	normal,
	staying,
	doubleH,
	SMG
};
struct Bullet
{
	Animation anim;

	p2Point<int> start;
	p2Point<int> end;

	int type;

	SDL_Rect start_rect;
	SDL_Rect end_rect;


	bool pathDone = false;

	bool Update(Application* app, int);

	void BreakingBrick(int num, int w, int h, Application* App);

};


class ModuleGun : public Module
{
public:

	ModuleGun(Application* app, bool start_enabled = true);
	~ModuleGun();

	SDL_Rect bulletRect;

	bool shootAvailable1 = true;
	bool shootAvailable2 = true;
	int type1;
	int type2;
	int maxShots1;
	int maxShots2;
	int bulletWidth;

	SDL_Texture* graphics;
	SDL_Texture* graphics2;

//	SDL_Texture* hookStick;

	Animation* normal_animation1;
	Animation* normal_animation2;

	Animation* staying_animation1;
	Animation* staying_animation2;

	Animation hook;
	Animation hook2;

	Animation staying1;
	Animation staying2;

	SDL_Rect hookStickRect;

public:
	bool Init();
	bool Start();
	update_status Update();
	bool CleanUp();

	void AddBullet(p2Point<int>, int, int);

	void Shoot(p2Point<int>, int);

public:
	p2List<Bullet*> activeBullet1;
	p2List<Bullet*> activeBullet2;

};