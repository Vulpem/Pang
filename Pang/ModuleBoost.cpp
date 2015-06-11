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
	graphics = NULL;
	graphics = App->textures->Load("./Image_Sources/PowerUps.png");
	if (graphics == NULL)
	{
		LOG("Could not load boosts graphics");
		return false;
	}
	pickedUp = App->audio->LoadFx("./Sounds/BoostPickUp.wav");
	pickedUpLife = App->audio->LoadFx("./Sounds/Life_Up.wav");
	return true;
}



update_status ModuleBoost::Update()
{
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
			//Blink before disappearing
			if (tmp->data->lifeTime > 450)
			{
				if (tmp->data->lifeTime / 8 % 2 == 0)
					App->render->Blit(graphics, tmp->data->position.x, tmp->data->position.y, &tmp->data->anim.GetCurrentFrame());
			}
			//Render as normal
			else
				App->render->Blit(graphics, tmp->data->position.x, tmp->data->position.y, &tmp->data->anim.GetCurrentFrame());
		}

		tmp = tmp_next;
	}
	// Rendering current boost
	if (App->player->IsEnabled())
	{
		switch (App->player->boost)
		{
		case doubleHook:
			player1Boost = { 16, 0, 16, 16 };
			App->render->Blit(graphics, 14 * TILE, 28 * TILE, &player1Boost);
			break;
		case stayingHook:
			player1Boost = { 0, 0, 16, 16 };
			App->render->Blit(graphics, 14 * TILE, 28 * TILE, &player1Boost);
			break;
		case SMG:
			player1Boost = { 192, 0, 16, 16 };
			App->render->Blit(graphics, 14 * TILE, 28 * TILE, &player1Boost);
			break;
		default:
			break;
		}
	}
	if (App->player2->IsEnabled())
	{
		switch (App->player2->boost)
		{
		case doubleHook:
			player2Boost = { 16, 0, 16, 16 };
			App->render->Blit(graphics, 44 * TILE, 29 * TILE, &player2Boost);
			break;
		case stayingHook:
			player2Boost = { 0, 0, 16, 16 };
			App->render->Blit(graphics, 44 * TILE, 29 * TILE, &player2Boost);
			break;
		case SMG:
			player2Boost = { 192, 0, 16, 16 };
			App->render->Blit(graphics, 44 * TILE, 29 * TILE, &player2Boost);
			break;
		default:
			break;
		}
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
	App->textures->Unload(graphics);

	return true;
}

void ModuleBoost::AddBoost(int x, int y, Boosts boostType)
{
	Boost* b = new Boost();
	int type;
	if (y > TILE * 23) { y = TILE * 23 - 4; }
	b->lifeTime = 0;

	if (boostType == none)
		type = rand() % 160 + 1;
	else
		type = boostType;
	if (type <= 20)
	{
		b->type = gun;
		b->anim.frames.PushBack({ 192, 0, 16, 16 });
		b->anim.speed = 0.0f;
	}
	else if (type <= 40)
	{
		b->type = stayingHook;
		b->anim.frames.PushBack({ 0, 0, 16, 16 });
	}
	else if (type <= 50)
	{
		b->type = life;
		b->anim.frames.PushBack({ 112, 0, 16, 16 });
	}
	else if (type <= 80)
	{
		b->type = doubleHook;
		b->anim.frames.PushBack({ 16, 0, 16, 16 });
	}
	else if (type <= 110)
	{
		b->type = bomb;
		b->anim.frames.PushBack({ 32, 0, 16, 16 });
		b->anim.frames.PushBack({ 48, 0, 16, 16 }); 
		b->anim.frames.PushBack({ 64, 0, 16, 16 });
		b->anim.speed = 10.0f;
	}
	else if (type <= 130)
	{
		b->type = shield;
		b->anim.frames.PushBack({ 128, 0, 16, 16 });
		b->anim.frames.PushBack({ 144, 0, 16, 16 });
		b->anim.frames.PushBack({ 160, 0, 16, 16 });
		b->anim.frames.PushBack({ 176, 0, 16, 16 });
		b->anim.frames.PushBack({ 160, 0, 16, 16 });
		b->anim.frames.PushBack({ 144, 0, 16, 16 });
		b->anim.speed = 0.2f;
	}
	else
	{
		b->type = pause;
		b->anim.frames.PushBack({ 96, 0, 16, 16 });
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
		lifeTime = 0;
		Fall(app);
	}
	if (app->player2->IsEnabled() && app->player2->position.x + 15 > position.x && app->player2->position.x + 15 < position.x + 32 && app->player2->position.y + 20 > position.y - 16 && app->player2->position.y + 20 < position.y + 16)
	{
		app->player2->score += 100;
		switch (type)
		{
		case stayingHook:
		{
			app->player2->boost = stayingHook; break;
		}
		case doubleHook:
		{
			app->player2->boost = doubleHook;  break;
		}
		case gun:
		{
			app->player2->boost = SMG; ; break;
		}
		case life:
		{
			app->scenePlay->lives2 += 1; app->audio->PlayFx(app->boosts->pickedUpLife); break;
		}
		case pause:
		{
			app->balls->PauseBoost();  break;
		}
		case bomb:
		{
			app->balls->BombBoost(); break;
		}
		case shield:
		{
			app->player2->shieldOn = true; break;
		}
		}
		app->audio->PlayFx(app->boosts->pickedUp);
		return false;
	}
	if (app->player->IsEnabled() && app->player->position.x + 15 > position.x && app->player->position.x + 15 < position.x + 32 && app->player->position.y + 20 > position.y - 16 && app->player->position.y + 20 < position.y + 16)
	{
		app->player->score += 100;
		switch (type)
		{
		case stayingHook:
		{
			app->player->boost = stayingHook; break;
		}
		case doubleHook:
		{
			app->player->boost = doubleHook; break;
		}
		case gun:
		{
			app->player->boost = SMG; break;
		}
		case life:
		{
			app->scenePlay->lives1 += 1; app->audio->PlayFx(app->boosts->pickedUpLife); break;
		}
		case pause:
		{
			app->balls->PauseBoost(); break;
		}
		case bomb:
		{
			app->balls->BombBoost(); break;
		}
		case shield:
		{
			app->player->shieldOn = true; break;
		}
		}
		app->audio->PlayFx(app->boosts->pickedUp);
		return false;
	}
	return true;
}

