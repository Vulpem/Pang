#include <math.h>
#include "Globals.h"
#include "Application.h"
#include "ModuleParticles.h"

ModuleParticles::ModuleParticles(Application* app, bool start_enabled) : Module(app, start_enabled), graphics(NULL)
{}

ModuleParticles::~ModuleParticles()
{}

// Load assets
bool ModuleParticles::Start()
{
	LOG("Loading particles");
	graphics = App->textures->Load("rtype/particles.png");

	// Explosion particle


	//explosion.fx = App->audio->LoadFx("rtype/explosion.wav");


	explosion.anim.frames.PushBack({274, 296, 33, 30});
	explosion.anim.frames.PushBack({313, 296, 33, 30});
	explosion.anim.frames.PushBack({346, 296, 33, 30});
	explosion.anim.frames.PushBack({382, 296, 33, 30});
	explosion.anim.frames.PushBack({419, 296, 33, 30});
	explosion.anim.frames.PushBack({457, 296, 33, 30});
	explosion.anim.loop = false;
	explosion.anim.speed = 0.3f;

	return true;
}

// Unload assets
bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");
	App->textures->Unload(graphics);
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
		else if(SDL_GetTicks() >= p->born)
		{
			App->renderer->Blit(graphics, p->position.x, p->position.y, &(p->anim.GetCurrentFrame()));
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

void ModuleParticles::AddParticle(const Particle& particle, int x, int y, Uint32 delay)
{
	Particle* p = new Particle(particle);
	p->born = SDL_GetTicks() + delay;
	p->position.x = x;
	p->position.y = y;

	active.add(p);
}

// -------------------------------------------------------------
// -------------------------------------------------------------

Particle::Particle() : fx(0), born(0), life(0), fx_played(false)
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) : anim(p.anim), position(p.position), speed(p.speed), fx_played(false)
{
	fx = p.fx;
	born = p.born;
	life = p.life;
}

Particle::~Particle()
{
}

bool Particle::Update()
{
	bool ret = true;

	if(life > 0)
	{
		if((SDL_GetTicks() - born) > life)
			ret = false;
	}
	else
		if(anim.Finished())
			ret = false;

	position.x += speed.x;
	position.y += speed.y;

	return ret;
}