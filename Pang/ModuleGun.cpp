#include "Globals.h"
#include "Application.h"
#include "ModuleGun.h"
#include "List.h"
#include "Module.h"
#include "ModuleBalls.h"


ModuleGun::ModuleGun(Application* app, bool start_enabled) :Module(app, start_enabled)
{
	
}
ModuleGun::~ModuleGun()
{

}

bool ModuleGun::Init()
{
	graphics = NULL;

	current_animation = &hook;
	hook.frames.PushBack({ 0, 0, 9, 199 });
	hook.frames.PushBack({ 9, 0, 9, 199 });

	hook.speed = 0.21f;
	hook.loop = true;

	return true;
}

bool ModuleGun::Start()
{
	graphics = App->textures->Load("./Image_Sources/Hook.png");
	if (graphics == NULL)
	{
		LOG("------------------Could not load gun graphics----------------------");
	}
	maxShots1 = maxShots2 = 1;

	bulletWidth = 3;
	return true;
}

void ModuleGun::AddBullet(p2Point<int> startPoint, int player)
{

	Bullet* b = new Bullet();

	b->start = startPoint;
	b->start.x = startPoint.x - bulletWidth / 2;
	b->end = startPoint;
	b->end.x = startPoint.x - bulletWidth / 2;
	b->end.y -= 24;

	//Creating the rectangle references
	b->start_rect.x = b->start.x;
	b->start_rect.y = b->start.y;
	b->start_rect.w = bulletWidth;
	b->start_rect.h = -30;

	b->end_rect.x = b->end.x;
	b->end_rect.y = b->end.y;
	b->end_rect.w = bulletWidth;
	b->end_rect.h = 1;
	if (player == 1)
		activeBullet1.add(b);
	else
		activeBullet2.add(b);
	App->particles->AddParticle(App->particles->shot, startPoint.x, startPoint.y - TILE * 4, 8, 8);
}

void ModuleGun::Shoot(p2Point<int> startPoint, int player)
{
	if (player == 1)
	{
		if (activeBullet1.count() < maxShots1)
		{
			AddBullet(startPoint, player);
			if (activeBullet1.count() == maxShots1)
				shootAvailable1 = false;
		}
	}
	else if (player == 2)
	{
		if (activeBullet2.count() < maxShots2)
		{
			AddBullet(startPoint, player);
			if (activeBullet2.count() == maxShots2)
				shootAvailable2 = false;
		}
	}

}

update_status ModuleGun::Update()
{

	p2List_item<Bullet*>* tmp = activeBullet1.getFirst();
	p2List_item<Bullet*>* tmp_next = activeBullet1.getFirst();
	while (tmp != NULL)
	{
		tmp_next = tmp->next;

		if (tmp->data->Update(App, 1) == false)
		{
			LOG("-- Destroying Bullet --\n");
			delete tmp->data;
			activeBullet1.del(tmp);
			shootAvailable1 = true;
		}

		else
		{
			App->render->Blit(graphics, tmp->data->end.x - 2, tmp->data->end.y, &current_animation->GetCurrentFrame());
		}

		tmp = tmp_next;
	}

	tmp = activeBullet2.getFirst();
	tmp_next = activeBullet2.getFirst();
	while (tmp != NULL)
	{
		tmp_next = tmp->next;

		if (tmp->data->Update(App, 2) == false)
		{
			LOG("-- Destroying Bullet --\n");
			delete tmp->data;
			activeBullet2.del(tmp);
			shootAvailable2 = true;
		}

		else
		{
			App->render->Blit(graphics, tmp->data->end.x - 2, tmp->data->end.y, &current_animation->GetCurrentFrame());
		}

		tmp = tmp_next;
	}

	return UPDATE_CONTINUE;
}



bool Bullet::Update(Application* app, int player)
{
	bool ret = true;
	if (app->maps->map[(end.y - 1) / 8][(end.x + 1)/ 8] == 1)
	{
		ret = false;
		int num = app->maps->lvl[app->scenePlay->currentLvl][(end.y - 1) / 8][end.x / 8];
		if (num != 1 && num != 2 && num != 0)
		{
			BreakingBrick(num, end.x / 8, (end.y - 1) / 8, app);
		}

	}
	else
	{
		end.y-=2;
		end_rect.h-=2;
		app->gun->hook.frames[0].h = -(end.y - start.y);
		app->gun->hook.frames[1].h = -(end.y - start.y);

/////////////////////////////////////////////////
		
		p2List_item<Ball*>* tmp = app->balls->ballsList.getFirst();

		while (tmp != NULL)
		{	
			if (!tmp->data->dead)
			{
				if ((tmp->data->position.y + tmp->data->radius >= end.y) &&
					(tmp->data->position.y <= start.y) &&
					((tmp->data->position.x - tmp->data->radius) < end.x + end_rect.w) &&
					(tmp->data->position.x + tmp->data->radius) > end.x)
				{
					tmp->data->dead = true;
					if (player == 1)
					{
						app->player->score += (50 * (4 - tmp->data->type));
					}
					else if (player == 2)
					{
						app->player2->score += (50 * (4 - tmp->data->type));
					}

					return false;
				}
			}

			tmp = tmp->next;
		}
////////////////////////////////////////////////

	}

	
	return ret;
}

bool ModuleGun::CleanUp()
{
	p2List_item<Bullet*>* tmp = activeBullet1.getFirst();
	p2List_item<Bullet*>* tmp_next = activeBullet1.getFirst();
	while (tmp != NULL)
	{
 		tmp_next = tmp->next;
		delete tmp->data;
		activeBullet1.del(tmp);
		tmp = tmp_next;
	}

	tmp = activeBullet2.getFirst();
	tmp_next = activeBullet2.getFirst();
	while (tmp != NULL)
	{
		tmp_next = tmp->next;
		delete tmp->data;
		activeBullet2.del(tmp);
		tmp = tmp_next;
	}
	shootAvailable1 = true;
	shootAvailable2 = true;
	App->textures->Unload(graphics);
	return true;
}

void Bullet::BreakingBrick(int num, int w, int h, Application* App)
{
	App->maps->map[h][w] = 0;

	App->particles->AddParticle(App->particles->breakingBrick[0], w*TILE, h*TILE);

	for (int x = -1; x <= 1; x += 2)
	{
		if (App->maps->lvl[App->scenePlay->currentLvl][h][w + x] == num && App->maps->map[h][w + x] == 1)
		{
			BreakingBrick(num, w + x, h, App);
		}
	}
	for (int y = -1; y <= 1; y += 2)
	{
		if (App->maps->lvl[App->scenePlay->currentLvl][h + y][w] == num && App->maps->map[h + y][w] == 1)
		{
			BreakingBrick(num, w, h + y, App);
		}
	}
}