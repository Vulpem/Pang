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
	return true;
}



update_status ModuleBoost::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		AddBoost(100, 101);
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
			App->renderer->DrawQuad(tmp->data->rect, 0, 0, 255, 130);
		}

		tmp = tmp_next;
	}

	return UPDATE_CONTINUE;
}


bool ModuleBoost::Cleanup()
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

	b->position.x = x;
	b->position.y = y;

	//Creating the rectangle references
	b->rect.x = b->position.x;
	b->rect.y = b->position.y;
	b->rect.w = 16;
	b->rect.h = 16;

	activeBoost.add(b);
}

void Boost::Fall(Application* app)
{

	position.y += 2;


	if (app->maps->map[(position.y + 16) / 8][(position.x + 8) / 8] != 0)
	{
		position.y = (((position.y + 16) / 8) - 2) * 8;
	}
	rect.y = position.y;
}

bool Boost::Update(Application* app)
{
	if (app->maps->map[(position.y + 16) / 8][(position.x + 8) / 8] == 0)
		Fall(app);
	return true;
}

