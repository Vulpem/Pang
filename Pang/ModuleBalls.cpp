
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

	return true;
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
			App->renderer->DrawQuad(pointer->data->start_rect, 50, 150, 50, 255);
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
//	speed.y = type * offsetDirection;
	speed.y = 0;

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
	//	speed.y = type * offsetDirection;
	speed.y = 0;

	//Creating the rect
	start_rect.x = x;
	start_rect.y = y;
	start_rect.w = 2 * type;
	start_rect.h = 2 * type;
}

bool Ball::Update()
{
	if (dead == false)
	{
		if (position.x > 300 || position.x < 10)
			speed.x *= -1;
		
		position.x += speed.x;
		return true;
	}
	else return false;
}
