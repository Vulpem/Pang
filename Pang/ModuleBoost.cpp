#include "Globals.h"
#include "Application.h"
#include "ModuleBoost.h"
#include "List.h"
#include "Module.h"

ModuleBoost::ModuleBoost(Application* app, bool start_enabled) :Module(app, start_enabled)
{

}

ModuleBoost::~ModuleBoost()
{

}


bool ModuleBoost::Start()
{
	graphics = App->textures->Load("./Image_Sources/PowerUps.png");
	return true;
}



update_status ModuleBoost::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		AddBoost(rand()%300 + 16, 101);
	}

	p2List_item<Boost*>* tmp = activeBoost.getFirst();
	p2List_item<Boost*>* tmp_next = activeBoost.getFirst();
	while (tmp != NULL)
	{
		tmp_next = tmp->next;

		if (tmp->data->Update(App) == false)
		{
			LOG("-- Destroying Boost --\n");
			delete tmp->data;
			activeBoost.del(tmp);
		}
		else
		{
			App->renderer->Blit(graphics, tmp->data->position.x, tmp->data->position.y, &tmp->data->anim.GetCurrentFrame());
		}

		tmp = tmp_next;
	}

	return UPDATE_CONTINUE;
}


bool ModuleBoost::CleanUp()
{
	p2List_item<Boost*>* tmp = activeBoost.getFirst();
	p2List_item<Boost*>* tmp_next = activeBoost.getFirst();
	while (tmp != NULL)
	{
		tmp_next = tmp->next;
		delete tmp->data;
		activeBoost.del(tmp);
		tmp = tmp_next;
	}


	return true;
}

void ModuleBoost::AddBoost(int x, int y)
{
	Boost* b = new Boost();

	if (y > TILE * 23) { y = TILE * 23 - 4; }
	b->lifeTime = 0;
	b->type = rand() % 4 + 1;
	switch (b->type)
	{
	case doubleHook:
	{
		b->anim.frames.PushBack({ 16, 0, 16, 16 }); break;
	}
	case bomb:
	{
		b->anim.frames.PushBack({ 32, 0, 16, 16 });
		b->anim.frames.PushBack({ 48, 0, 16, 16 }); 
		b->anim.frames.PushBack({ 64, 0, 16, 16 }); break;
		b->anim.speed = 10.0f;
	}
	case pause:
	{
		b->anim.frames.PushBack({ 96, 0, 16, 16 }); break;
	}
	case life:
	{
		b->anim.frames.PushBack({ 112, 0, 16, 16 }); break;
	}
	}
	b->position.x = x;
	b->position.y = y;

	activeBoost.add(b);
}

void Boost::Fall(Application* app)
{

	position.y += 2;


	if (app->maps->map[(position.y + 16) / 8][(position.x + 8) / 8] != 0)
	{
		position.y = (((position.y + 16) / 8) - 2) * 8;
	}
}

bool Boost::Update(Application* app)
{
	lifeTime++;
	if (lifeTime >= 500) { return false; }
	if (app->maps->map[(position.y + 16) / 8][(position.x + 8) / 8] == 0)
	{
		Fall(app);
	}
	if (app->player->position.x + 15 > position.x && app->player->position.x + 15 < position.x + 32 && app->player->position.y + 20 > position.y - 16 && app->player->position.y + 20 < position.y + 16)
	{
		switch (type)
		{
		case doubleHook:
		{
			app->player->boost = doubleHook; break;
		}
		case life:
		{
			app->backgroundPlay->lives += 1; break;
		}
		case pause:
		{
			app->balls->PauseBoost(); break;
		}
		case bomb:
		{
			app->balls->BombBoost();
		}
		}
	return false;
	}
	return true;
}

