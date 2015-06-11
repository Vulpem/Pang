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

	hookStickRect = { 36, 0, 9, 191 };
	hook.frames.PushBack({ 0, 0, 9, 199 });
	hook.frames.PushBack({ 9, 0, 9, 199 });

	hook.speed = 0.21f;
	hook.loop = true;

	hook2.frames.PushBack({ 0, 0, 9, 199 });
	hook2.frames.PushBack({ 9, 0, 9, 199 });

	hook2.speed = 0.21f;
	hook2.loop = true;

	staying1.frames.PushBack({ 18, 0, 9, 199 });
	staying1.frames.PushBack({ 27, 0, 9, 199 });

	staying1.speed = 0.21f;
	staying1.loop = true;

	staying2.frames.PushBack({ 18, 0, 9, 199 });
	staying2.frames.PushBack({ 27, 0, 9, 199 });

	staying2.speed = 0.21f;
	staying2.loop = true;

	normal_animation1 = &hook;
	normal_animation2 = &hook2;

	staying_animation1 = &staying1;
	staying_animation2 = &staying2;

	bulletRect = { 0, 0, 4, 8 };
	return true;
}

bool ModuleGun::Start()
{
	graphics = App->textures->Load("./Image_Sources/Hook.png");
	if (graphics == NULL)
	{
		LOG("------------------Could not load gun graphics----------------------");
	}
	graphics2 = App->textures->Load("./Image_Sources/Hook.png");

	bulletTexture = App->textures->Load("./Image_Sources/Bullet.png");
	if (bulletTexture == NULL)
	{
		LOG("-----------------Could not load bullet graphics------------------");
	}
	maxShots1 = maxShots2 = 1;
	type1 = type2 = normal;
	bulletWidth = 3;
	hookPathDone = App->audio->LoadFx("Sounds/hook_enganxat.wav");
	breakingBrick = App->audio->LoadFx("Sounds/BreakingBrick.wav");
	bulletHitBrick = App->audio->LoadFx("Sounds/bulletHitBrick.wav");
	return true;
}

void ModuleGun::AddBullet(p2Point<int> startPoint, int player, int _type)
{

	Bullet* b = new Bullet();

	b->start = startPoint;
	b->start.x = startPoint.x - bulletWidth / 2;
	b->end = startPoint;
	b->end.x = startPoint.x - bulletWidth / 2;
	b->end.y -= 24;
	b->type = _type;

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
	if (_type == SMG)
	{
		App->particles->AddParticle(App->particles->gunShot, startPoint.x, startPoint.y - TILE * 4, 8, 8);
	}
	else
	{
		App->particles->AddParticle(App->particles->shot, startPoint.x, startPoint.y - TILE * 4, 8, 8);
	}
}

void ModuleGun::Shoot(p2Point<int> startPoint, int player)
{
	if (player == 1)
	{
		if (activeBullet1.count() < maxShots1)
		{
			AddBullet(startPoint, player, type1);
			if (activeBullet1.count() == maxShots1)
				shootAvailable1 = false;
		}
	}
	else if (player == 2)
	{
		if (activeBullet2.count() < maxShots2)
		{
			AddBullet(startPoint, player, type2);
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
			if (tmp->data->pathDone)
			{
				hookStickRect.h = tmp->data->start.y - tmp->data->end.y;
				if (tmp->data->stickTimer < 180)
					hookStickRect.x = 36;
				else if (tmp->data->stickTimer < 300)
					hookStickRect.x = 45;
				else
					hookStickRect.x = 54;
				App->render->Blit(graphics, tmp->data->end.x - 2, tmp->data->end.y, &hookStickRect);
			}
			else
			{
				if (tmp->data->type != staying && tmp->data->type != SMG)
					App->render->Blit(graphics, tmp->data->end.x - 2, tmp->data->end.y, &normal_animation1->GetCurrentFrame());
				else if (tmp->data->type == staying)
					App->render->Blit(graphics, tmp->data->end.x - 2, tmp->data->end.y, &staying_animation1->GetCurrentFrame());
				else if (tmp->data->type == SMG)
				{
					App->render->Blit(bulletTexture, tmp->data->end.x - 6, tmp->data->end.y, &bulletRect);
					App->render->Blit(bulletTexture, tmp->data->end.x + 3,tmp->data->end.y, &bulletRect);
				}
			}
		}
		tmp = tmp_next;
	}

	p2List_item<Bullet*>* tmp2 = activeBullet2.getFirst();
	p2List_item<Bullet*>* tmp_next2 = activeBullet2.getFirst();
	while (tmp2 != NULL)
	{
		tmp_next2 = tmp2->next;

		if (tmp2->data->Update(App, 2) == false)
		{
			LOG("-- Destroying Bullet --\n");
			delete tmp2->data;
			activeBullet2.del(tmp2);
			shootAvailable2 = true;
		}

		else
		{
			if (tmp2->data->pathDone)
			{
				hookStickRect.h = tmp2->data->start.y - tmp2->data->end.y;
				if (tmp2->data->stickTimer < 180)
					hookStickRect.x = 36;
				else if (tmp2->data->stickTimer < 300)
					hookStickRect.x = 45;
				else
					hookStickRect.x = 54;
				App->render->Blit(graphics, tmp2->data->end.x - 2, tmp2->data->end.y, &hookStickRect);
			}
			else
			{
				if (tmp2->data->type != staying && tmp2->data->type != SMG)
					App->render->Blit(graphics, tmp2->data->end.x - 2, tmp2->data->end.y, &normal_animation2->GetCurrentFrame());
				else if (tmp2->data->type == staying)
					App->render->Blit(graphics, tmp2->data->end.x - 2, tmp2->data->end.y, &staying_animation2->GetCurrentFrame());
				else if (tmp2->data->type == SMG)
				{
					App->render->Blit(bulletTexture, tmp2->data->end.x - 6, tmp2->data->end.y, &bulletRect);
					App->render->Blit(bulletTexture, tmp2->data->end.x + 3, tmp2->data->end.y, &bulletRect);
				}
			}

		}

		tmp2 = tmp_next2;
	}

	return UPDATE_CONTINUE;
}

bool Bullet::Update(Application* app, int player)
{
	bool ret = true;
	if (pathDone)
	{
		if (stickTimer == 0)
		{
			app->audio->PlayFx(app->gun->hookPathDone);
		}
		stickTimer++;
		if (stickTimer >= 360)
			return false;
	}
	if (app->maps->map[(end.y - 1) / 8][(end.x + 1)/ 8] == 1)
	{
		if (type == stayingHook)
		{
			pathDone = true;
		}
		else
		{
			ret = false;
			if (type == gun)
			{
				app->audio->PlayFx(app->gun->bulletHitBrick);
			}
		}
		if (type != gun)
		{
			int num = app->maps->lvl[app->scenePlay->currentLvl][(end.y - 1) / 8][end.x / 8];
			if (num != 1 && num != 2 && num != 0)
			{
				ret = false;
				BreakingBrick(num, end.x / 8, (end.y - 1) / 8, app);
				if (player == 1)
				{
					app->player->score += 100;
				}
				else
				{
					app->player2->score += 100;
				}
				app->audio->PlayFx(app->gun->breakingBrick);
			}
		}


	}
	else
	{
		if (type != gun)
		{
			end.y -= 2;
			end_rect.h -= 2;
			if (player == 1)
			{
				if (type == normal)
				{
					app->gun->hook.frames[0].h = -(end.y - start.y);
					app->gun->hook.frames[1].h = -(end.y - start.y);
				}
				else if (type == staying)
				{
					app->gun->staying1.frames[0].h = -(end.y - start.y);
					app->gun->staying1.frames[1].h = -(end.y - start.y);
				}
			}
			else if (player == 2)
			{
				if (type == normal)
				{
					app->gun->hook2.frames[0].h = -(end.y - start.y);
					app->gun->hook2.frames[1].h = -(end.y - start.y);
				}
				else if (type == staying)
				{
					app->gun->staying2.frames[0].h = -(end.y - start.y);
					app->gun->staying2.frames[1].h = -(end.y - start.y);
				}
			}
		}
		else if (type == gun)
		{
			end.y -= 4;
		}
	}
	/////////////////////////////////////////////////.
	//Checking collisions with balls
	if (app->maps->map[(end.y - 1) / 8][(end.x + 1) / 8] != 1 || type == stayingHook)
	{
		p2List_item<Ball*>* tmp = app->balls->ballsList.getFirst();

		while (tmp != NULL)
		{
			if (tmp->data->dead == false)
			{
				if (type != gun)
				{
					if ((tmp->data->position.y + tmp->data->radius >= end.y) &&
						(tmp->data->position.y <= start.y) &&
						((tmp->data->position.x - tmp->data->radius) < end.x + end_rect.w) &&
						(tmp->data->position.x + tmp->data->radius) > end.x)
					{
						tmp->data->dead = true;
						if (player == 1)
						{
							if (IncreaseScore(app->player, tmp->data->type))
							{
								RenderScore(app, tmp->data->position, tmp->data->type, app->player->multiplier);
							}
						}
						else if (player == 2)
						{
							if(IncreaseScore(app->player2, tmp->data->type))
							{
								RenderScore(app, tmp->data->position, tmp->data->type, app->player2->multiplier);
							}
						}

						return false;
					}
				}
				else if (type == gun)
				{
					if ((tmp->data->position.y + tmp->data->radius >= end.y) &&
						(tmp->data->position.y <= end.y + 8) &&
						((tmp->data->position.x - tmp->data->radius) < end.x + 7) &&
						(tmp->data->position.x + tmp->data->radius) > end.x - 6)

					{
						tmp->data->dead = true;
						if (player == 1)
						{
							if (IncreaseScore(app->player, tmp->data->type))
							{
								RenderScore(app, tmp->data->position, tmp->data->type, app->player->multiplier);
							}
						}
						else if (player == 2)
						{
							if (IncreaseScore(app->player2, tmp->data->type))
							{
								RenderScore(app, tmp->data->position, tmp->data->type, app->player2->multiplier);
							}
						}

						return false;
					}
				}

			}

			tmp = tmp->next;
		}
	}
////////////////////////////////////////////////
	
	return ret;
}

bool Bullet::IncreaseScore(ModulePlayer* player, int ballType)
{
	bool ret = false;
	if (ballType != player->lastHitBall)
	{
		player->lastHitBall = ballType;
		player->multiplier = 1;
	}
	else
	{
		if (player->multiplier < 8)
		{
			player->multiplier = player->multiplier * 2;
		}
		ret = true;
	}
	player->score += (50 * (4 - ballType) * player->multiplier);
	return ret;
}

bool Bullet::IncreaseScore(ModulePlayer2* player, int ballType)
{
	bool ret = false;
	if (ballType != player->lastHitBall)
	{
		player->lastHitBall = ballType;
		player->multiplier = 1;
	}
	else
	{
		if (player->multiplier < 8)
		{
			player->multiplier = player->multiplier * 2;
		}
		ret = true;
	}
	player->score += (50 * (4 - ballType) * player->multiplier);
	return ret;
}

void Bullet::RenderScore(Application* App, p2Point<float>& point, int ballType, int multiplier)
{
	switch (ballType)
	{
		case huge:
		{
			switch (multiplier)
			{
			case 2:
			{
				App->particles->AddParticle(App->particles->points[0], point.x, point.y, 6, 16); break;
			}
			case 4:
			{
				App->particles->AddParticle(App->particles->points[1], point.x, point.y, 6, 16); break;
			}
			case 8:
			{
				App->particles->AddParticle(App->particles->points[3], point.x, point.y, 6, 16); break;
			}
			}
			break;
		}
		case big:
		{
			switch (multiplier)
			{
			case 2:
			{
				App->particles->AddParticle(App->particles->points[1], point.x, point.y, 6, 12); break;
			}
			case 4:
			{
				App->particles->AddParticle(App->particles->points[3], point.x, point.y, 6, 12); break;
			}
			case 8:
			{
				App->particles->AddParticle(App->particles->points[5], point.x, point.y, 6, 12); break;
			}
			}
			break;
		}
		case medium:
		{
			switch (multiplier)
			{
			case 2:
			{
				App->particles->AddParticle(App->particles->points[2], point.x, point.y, 6, 8); break;
			}
			case 4:
			{
				App->particles->AddParticle(App->particles->points[4], point.x, point.y, 6, 8); break;
			}
			case 8:
			{
				App->particles->AddParticle(App->particles->points[6], point.x, point.y, 8, 8); break;
			}
			}
			break;
		}
		case little:
		{
			switch (multiplier)
			{
			case 2:
			{
				App->particles->AddParticle(App->particles->points[3], point.x, point.y, 6, 4); break;
			}
			case 4:
			{
				App->particles->AddParticle(App->particles->points[5], point.x, point.y, 6, 4); break;
			}
			case 8:
			{
				App->particles->AddParticle(App->particles->points[7], point.x, point.y, 8, 4); break;
			}
			}
			break;
		}
	}

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

	p2List_item<Bullet*>* tmp2 = activeBullet2.getFirst();
	p2List_item<Bullet*>* tmp_next2 = activeBullet2.getFirst();

	while (tmp2 != NULL)
	{
		tmp_next2 = tmp2->next;
		delete tmp2->data;
		activeBullet2.del(tmp2);
		tmp2 = tmp_next2;
	}
	shootAvailable1 = true;
	shootAvailable2 = true;
	App->textures->Unload(graphics);
	App->textures->Unload(graphics2);
	App->textures->Unload(bulletTexture);
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