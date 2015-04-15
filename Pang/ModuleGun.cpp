#include "Globals.h"
#include "Application.h"
#include "ModuleGun.h"
#include "List.h"

bool ModuleGun::Start()
{
	return true;
}

void ModuleGun::AddBullet(p2Point<int> startPoint)
{
	Bullet* b = new Bullet();

	b->start = startPoint;
	b->end = startPoint;

	//Creating the rectangle references
	b->start_rect.x = b->start.x;
	b->start_rect.y = b->start.y;
	b->start_rect.w = 10;
	b->start_rect.h = 10;

	b->end_rect.x = b->end.x;
	b->end_rect.y = b->end.y;
	b->end_rect.w = 10;
	b->end_rect.h = 10;
}

update_status ModuleGun::Update()
{
	p2List_item<Bullet*>* tmp = active.getFirst();
	p2List_item<Bullet*>* tmp_next  = active.getFirst();
	while (tmp != NULL)
	{
		tmp_next = tmp->next;
		tmp->data->end.y--;
	}

	tmp = tmp_next;
	return UPDATE_CONTINUE;
}

//ModuleGun(Application* app);
//~ModuleGun();



