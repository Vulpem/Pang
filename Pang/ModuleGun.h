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
	//Bullet atributes
	p2Point<int> start;
	p2Point<int> end;

	int type;

	//Graphics atributes
	SDL_Rect start_rect;
	SDL_Rect end_rect;

	//Utils atributes
	bool pathDone = false;
	int	stickTimer = 0;

	//Methods
	bool Update(Application* app, int);
	bool IncreaseScore(ModulePlayer* player, int ballType);
	bool IncreaseScore(ModulePlayer2* player, int ballType);
	void RenderScore(Application* App, p2Point<float>& point, int ballType, int multiplier);
	void BreakingBrick(int num, int w, int h, Application* App);

};


class ModuleGun : public Module
{
public:

	ModuleGun(Application* app, bool start_enabled = true);
	~ModuleGun();


	p2List<Bullet*> activeBullet1;
	p2List<Bullet*> activeBullet2;

	bool shootAvailable1 = true;
	bool shootAvailable2 = true;
	int type1;
	int type2;
	int maxShots1;
	int maxShots2;
	int bulletWidth;
	unsigned int hookPathDone;
	unsigned int breakingBrick;
	unsigned int bulletHitBrick;

	//Graphics atributes
	SDL_Rect bulletRect;

	SDL_Texture* graphics;
	SDL_Texture* graphics2;
	SDL_Texture* bulletTexture;

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

	//Application methods
	bool Init();
	bool Start();
	update_status Update();
	bool CleanUp();

	//Logistic methods
	void AddBullet(p2Point<int>, int, int);

	void Shoot(p2Point<int>, int);

};