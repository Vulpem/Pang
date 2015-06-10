#pragma once
#include "Module.h"
#include "ModuleAnimation.h"
#include "Globals.h"
#include "p2Point.h"
#include "List.h"

struct Particle
{
	Animation anim;
	unsigned int fx;
	p2Point<int> position;
	bool fx_played;

	int offsetX;
	int offsetY;

	Particle();
	Particle(const Particle& p);
	~Particle();
	bool Update();
};

class ModuleParticles : public Module
{
public:
	ModuleParticles(Application* app, bool start_enabled = true);
	~ModuleParticles();

	bool Init();
	bool Start();
	update_status Update();
	bool CleanUp();

	void Clear();
	void AddParticle(const Particle& particle, int x, int y, int _offsetX = 0, int _offsetY = 0, Uint32 delay = 0);

private:

	SDL_Texture* explosionGraphics;
	p2List<Particle*> active;

public:

	Particle explosion[3][4];
	Particle shot;
	Particle breakingBrick[5];
	Particle points[8];

};