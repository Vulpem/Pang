
#include "Globals.h"
#include "Application.h"
#include "ModuleBalls.h"
#include "Application.h"

#include <iostream>

ModuleBalls::ModuleBalls(Application* app) : Module(app)
{

}

ModuleBalls::~ModuleBalls()
{

}

void ModuleBalls::AddBall(int position_x, int position_y, int _type, int _direction)
{
	Ball* newBall = new Ball(position_x, position_y, _type, _direction);
	ballsList.add(newBall);
}

bool ModuleBalls::Start()
{
	ballsRects[red][huge].x = ballsRects[red][huge].y = 0; ballsRects[red][huge].w = ballsRects[red][huge].h = 48;
	ballsRects[red][big].x = ballsRects[red][big].y = 48; ballsRects[red][big].w = ballsRects[red][big].h = 32;
	ballsRects[red][medium].x = ballsRects[red][medium].y = 80; ballsRects[red][medium].w = ballsRects[red][medium].h = 16;
	ballsRects[red][little].x = ballsRects[red][little].y = 76; ballsRects[red][little].w = ballsRects[red][little].h = 8;

	ballsRects[blue][huge].x = 0; ballsRects[blue][huge].y = 48; ballsRects[blue][huge].w = ballsRects[blue][huge].h = 48;
	ballsRects[blue][big].x = 48; ballsRects[blue][big].y = 48; ballsRects[blue][big].w = ballsRects[blue][big].h = 32;
	ballsRects[blue][medium].x = 80; ballsRects[blue][medium].y = 48; ballsRects[blue][medium].w = ballsRects[blue][medium].h = 16;
	ballsRects[blue][little].x = 76; ballsRects[blue][little].y = 48; ballsRects[blue][little].w = ballsRects[blue][little].h = 8;

	ballsRects[green][huge].x = 0;  ballsRects[green][huge].y = 96; ballsRects[green][huge].w = ballsRects[green][huge].h = 48;
	ballsRects[green][big].x = 48;  ballsRects[green][big].y = 96; ballsRects[green][big].w = ballsRects[green][big].h = 32;
	ballsRects[green][medium].x = 80;  ballsRects[green][medium].y = 96; ballsRects[green][medium].w = ballsRects[green][medium].h = 16;
	ballsRects[green][little].x = 76;  ballsRects[green][little].y = 96; ballsRects[green][little].w = ballsRects[green][little].h = 8;

	return true;
}

update_status ModuleBalls::PreUpdate()
{
	ballsGraphics = App->textures->Load(". / Image_Sources / Balls.png");
	return UPDATE_CONTINUE;
}

update_status ModuleBalls::Update()
{
	p2List_item<Ball*>* pointer = ballsList.getFirst();
	p2List_item<Ball*>* pointer_next = ballsList.getFirst();

	while (pointer!=NULL)
	{
	//	Ball* ball = pointer->data;
		pointer_next = pointer->next;

		if (pointer->data->Update() == false)
		{
			std::cout << "-- Destroying ball --" << std::endl;
			if (pointer->data->type > 1)
			{
			Ball* newBall1 = new Ball(pointer->data, -1);
			Ball* newBall2 = new Ball(pointer->data, 1);

			ballsList.add(newBall1);
			ballsList.add(newBall2);
			}


		//	delete ball;
			ballsList.del(pointer);
		}
		else
		{
			pointer->data->start_rect.x = pointer->data->position.x;
			pointer->data->start_rect.y = pointer->data->position.y;
			App->renderer->DrawQuad(pointer->data->start_rect, 255, 255, 0, 255);
			App->renderer->Blit(ballsGraphics, pointer->data->position.x, pointer->data->position.y, &ballsRects[red][pointer->data->type]);
			
		}
		
		pointer = pointer_next;
	}
	return UPDATE_CONTINUE;
}

Ball::Ball(Ball* parent, int offsetDirection)
{
	position.y = parent->position.y;
	position.x = parent->position.x + (parent->offset * offsetDirection);

	type = parent->type - 1;
	if (type == medium)
		offset = 4;
	else
		offset = 8;

	speed.x = type * offsetDirection;
	speed.y = (type + 4) / 2;
	//speed.y = 0;

	//Creating the rect

	start_rect.x = position.x;
	start_rect.y = position.y;
	start_rect.w = 2 * type;
	start_rect.h = 2 * type;


}



Ball::Ball(int x, int y, int _type, int direction)
{
	position.y = y;
	position.x = x;
	type = _type;

	if (type == medium)
		offset = 4;
	else
		offset = 8;

	speed.x = type * direction;
	speed.y = (type + 4) / 2;
	//speed.y = 0;

	//Creating the rect
	start_rect.x = x;
	start_rect.y = y;
	start_rect.w = 2 * type;
	start_rect.h = 2 * type;
}

bool Ball::Update()
{

	// S'ha d'ajustar Speed a tiles (ja mentenc jo :D ja t'explicare)
	if (dead == false)
	{
		if (position.x > 364 || position.x < 8)
			speed.x *= -1;
		if (position.y > SCREEN_HEIGHT - 5 * TILE)
		{
			speed.y = -15;
			position.y = (SCREEN_HEIGHT - 5 * TILE) + 1;
		}

		position.x += speed.x;
		position.y += speed.y;
		speed.y += 1;
		return true;
	}
	else return false;
}
