#include <math.h>
#include "Globals.h"
#include "Application.h"
#include "ModuleParticles.h"

ModuleParticles::ModuleParticles(Application* app, bool start_enabled) : Module(app, start_enabled), explosionGraphics(NULL)
{}

ModuleParticles::~ModuleParticles()
{}

bool ModuleParticles::Init()
{
	
	// Explosion particle
	shot.anim.frames.PushBack({ 0, 90, 16, 16 });
	shot.anim.frames.PushBack({ 16, 90, 16, 16 });
	shot.anim.frames.PushBack({ 32, 90, 16, 16 });
	shot.anim.frames.PushBack({ 48, 90, 16, 16 });
	shot.anim.loop = false;
	shot.anim.speed = 0.4f;
	shot.fx = App->audio->LoadFx("Sounds/Hook.wav");

	explosion[0][0].fx = App->audio->LoadFx("Sounds/Explosion.ogg");
	for (int n = 0; n < 3; n++)
	{
		explosion[n][huge].anim.frames.PushBack({ 90, 30 * n, 30, 30 });
		explosion[n][huge].anim.frames.PushBack({ 60, 30 * n, 30, 30 });
		explosion[n][huge].anim.frames.PushBack({ 30, 30 * n, 30, 30 });
		explosion[n][huge].anim.frames.PushBack({ 0, 30 * n, 30, 30 });

		explosion[n][big].anim.frames.PushBack({ 90, 30 * n, 30, 30 });
		explosion[n][big].anim.frames.PushBack({ 60, 30 * n, 30, 30 });
		explosion[n][big].anim.frames.PushBack({ 30, 30 * n, 30, 30 });
		explosion[n][big].anim.frames.PushBack({ 0, 30 * n, 30, 30 });

		explosion[n][medium].anim.frames.PushBack({ 90, 30 * n, 30, 30 });
		explosion[n][medium].anim.frames.PushBack({ 60, 30 * n, 30, 30 });
		explosion[n][medium].anim.frames.PushBack({ 30, 30 * n, 30, 30 });
		explosion[n][medium].anim.frames.PushBack({ 0, 30 * n, 30, 30 });

		explosion[n][little].anim.frames.PushBack({ 240, 30 * n, 30, 30 });
		explosion[n][little].anim.frames.PushBack({ 210, 30 * n, 30, 30 });
		explosion[n][little].anim.frames.PushBack({ 180, 30 * n, 30, 30 });
		explosion[n][little].anim.frames.PushBack({ 150, 30 * n, 30, 30 });
		for (int m = 0; m < 4; m++)
		{
			explosion[n][m].anim.loop = false;
			explosion[n][m].anim.speed = 0.2f;
			explosion[n][m].fx = explosion[0][0].fx;
		}
	}

	/////////////////////
	//Animations Bricks//
	/////////////////////

	for (int n = 0; n++; n < 5)
	{
		breakingBrick[n].anim.loop = false;
		breakingBrick[n].anim.speed = 0.2f;
		breakingBrick[n].fx = App->audio->LoadFx("Sounds/Explosion.ogg");
	}
	//brick frames
	breakingBrick[0].anim.frames.PushBack({ 65, 88, 8, 8 });
	breakingBrick[0].anim.frames.PushBack({ 73, 88, 8, 8 });
	breakingBrick[0].anim.frames.PushBack({ 81, 88, 8, 8 });
	for (int n = 1; n < 5; n++)
	{
		breakingBrick[n].anim.frames.PushBack({ 57 + n * 8, 80, 8, 8 });
		breakingBrick[n].anim.frames.PushBack({ 89 + n * 8, 80, 8, 8 });
		breakingBrick[n].anim.frames.PushBack({ 121 + n * 8, 80, 8, 8 });
	}

	return true;
}

// Load assets
bool ModuleParticles::Start()
{
	LOG("Loading particles");
	explosionGraphics = App->textures->Load("Image_Sources/Explosions.png");
	if (explosionGraphics == NULL)
	{
		LOG("Error loading particle graphics");
		assert(false);
		return false;
	}

	return true;
}

// Unload assets
bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");
	App->textures->Unload(explosionGraphics);
	return true;
}

// Update: draw background
update_status ModuleParticles::Update()
{
	p2List_item<Particle*>* tmp = active.getFirst();
	p2List_item<Particle*>* tmp_next = active.getFirst();


	while(tmp != NULL)
	{
		Particle* p = tmp->data;
		tmp_next = tmp->next;

		if(p->Update() == false)
		{
			delete p;
			active.del(tmp);
		}
		else
		{
			App->renderer->Blit(explosionGraphics, p->position.x, p->position.y, &(p->anim.GetCurrentFrame()), p->offsetX, p->offsetY);
			if(p->fx_played == false)
			{
				p->fx_played = true;
				App->audio->PlayFx(p->fx);
			}
		}

		tmp = tmp_next;
	}

	return UPDATE_CONTINUE;
}

void ModuleParticles::Clear()
{
	p2List_item<Particle*>* tmp = active.getFirst();
	p2List_item<Particle*>* tmp_next;
	while (tmp != NULL)
	{
		tmp_next = tmp->next;
		delete tmp->data;
		active.del(tmp);
		tmp = tmp_next;
	}
}

void ModuleParticles::AddParticle(const Particle& particle, int x, int y, int _offsetX, int _offsetY, Uint32 delay)
{
 	Particle* p = new Particle(particle);
	p->position.x = x;
	p->position.y = y;
	p->offsetX = _offsetX;
	p->offsetY = _offsetY;

	active.add(p);
	LOG("\nCreated New Particle\n")
}

// -------------------------------------------------------------
// -------------------------------------------------------------

Particle::Particle() : fx(0), fx_played(false)
{
	position.SetToZero();
}

Particle::Particle(const Particle& p) : anim(p.anim), position(p.position), fx_played(false)
{
	fx = p.fx;
}

Particle::~Particle()
{
}

bool Particle::Update()
{
	bool ret = true;

		if(anim.Finished())
			ret = false;

	return ret;
}