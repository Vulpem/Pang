#include "Globals.h"
#include "Application.h"
#include "ModuleGun.h"
#include "List.h"
#include "Maps2.h"

#include <iostream>

bool ModuleGun::Start()
{
	return true;
}

void ModuleGun::AddBullet(p2Point<int> startPoint)
{
	Bullet* b = new Bullet();

	b->start = startPoint;
	b->end = startPoint;
	b->end.y -= 24;

	//Creating the rectangle references
	b->start_rect.x = b->start.x;
	b->start_rect.y = b->start.y;
	b->start_rect.w = 1;
	b->start_rect.h = 1;

	b->end_rect.x = b->end.x;
	b->end_rect.y = b->end.y;
	b->end_rect.w = 1;
	b->end_rect.h = 1;

	active.add(b);
}

void ModuleGun::Shoot(p2Point<int> startPoint)
{
	if (active.count() == 0)
	{
		AddBullet(startPoint);
		shootAvailable = false;
	}
}

update_status ModuleGun::Update()
{

	p2List_item<Bullet*>* tmp = active.getFirst();
	p2List_item<Bullet*>* tmp_next = active.getFirst();
	while (tmp != NULL)
	{
		Bullet* b = tmp->data;
		tmp_next = tmp->next;

		if (b->Update() == false)
		{
			std::cout << "Destroying Bullet";
			delete b;
			active.del(tmp);
			shootAvailable = true;
		}
		else
		{
			App->renderer->DrawQuad(tmp->data->end_rect, 0, 0, 255, 130);
			App->renderer->DrawQuad(tmp->data->start_rect, 255, 0, 0, 255);
		}

		tmp = tmp_next;
	}

	return UPDATE_CONTINUE;
}

ModuleGun::ModuleGun(Application* app) :Module(app)
{

}
ModuleGun::~ModuleGun()
{

}

bool Bullet::Update()
{
	bool ret = true;
	std::cout << "Updating bullet";
	if (map2[(end.y - 1) / 8][end.x/8] == 1)
	{
		ret = false;
	}
	else
	{
		end.y--;
		end_rect.y = end.y;
	}

	
	return ret;
}



