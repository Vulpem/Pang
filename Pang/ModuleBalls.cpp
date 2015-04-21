
#include "Globals.h"
#include "Application.h"
#include "ModuleBalls.h"

#include <iostream>

////  CLASS BALL  //////////////////////////////////////

//Create a child ball
Ball::Ball(Ball* parent, int offsetDirection)
{
	position.y = parent->position.y;
	position.x = parent->position.x + (parent->offset * offsetDirection);
	type = parent->type - 1;
	CreateBall(offsetDirection);
}

//Create the ball when initializing the level
Ball::Ball(int x, int y, int _type, int direction)
{
	position.y = y;
	position.x = x;
	type = _type;
	CreateBall(direction);
}

void Ball::CreateBall(int direction)
{
	switch (type)
	{
		case huge:
		{speed.y = -3.0f; speed.x = 1.5f * direction; radius = 24; YBaseSpeed = -6.9f; offset = 8; break; }
		case big:
		{speed.y = -2.5f; speed.x = 2.0f * direction; radius = 16; YBaseSpeed = -6.5f; offset = 8; break; }
		case medium:
		{speed.y = -2.0f; speed.x = 1.5f * direction; radius = 8; YBaseSpeed = -6.0f; offset = 4; break; }
		case little:
		{speed.y = -2.0f; speed.x = 1.5f * direction; radius = 4; YBaseSpeed = -5.0f; offset = 0; break; }
	}

	//Creating the rect
	start_rect.x = position.x - radius;
	start_rect.y = position.y - radius;
	start_rect.w = radius * 2;
	start_rect.h = radius * 2;
}


bool Ball::Update(bool pause)
{
	// S'ha de mirar si s'ha d'ajustar Speed a tiles (ja mentenc jo :D ja t'explicare)
	if (dead == false)
	{
		if (pause == false)
		{
			position.x += speed.x;
			position.y += speed.y;
			speed.y += 0.2;
		}
		return true;
	}
	return false;
}



////////////////////
//  MODULE BALLS ///////////////////////
////////////////////

ModuleBalls::ModuleBalls(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleBalls::~ModuleBalls()
{
}


//Module methods

bool ModuleBalls::Start()
{
	LOG("Initializing Balls");
	ballsRects[red][huge].x = 0;		ballsRects[red][huge].y = 0;		ballsRects[red][huge].w = ballsRects[red][huge].h = 48;
	ballsRects[red][big].x = 48;		ballsRects[red][big].y = 0;			ballsRects[red][big].w = ballsRects[red][big].h = 32;
	ballsRects[red][medium].x = 80;		ballsRects[red][medium].y = 0;		ballsRects[red][medium].w = ballsRects[red][medium].h = 16;
	ballsRects[red][little].x = 96;		ballsRects[red][little].y = 0;		ballsRects[red][little].w = ballsRects[red][little].h = 8;

	ballsRects[blue][huge].x = 0;		ballsRects[blue][huge].y = 48;		ballsRects[blue][huge].w = ballsRects[blue][huge].h = 48;
	ballsRects[blue][big].x = 48;		ballsRects[blue][big].y = 48;		ballsRects[blue][big].w = ballsRects[blue][big].h = 32;
	ballsRects[blue][medium].x = 80;	ballsRects[blue][medium].y = 48;	ballsRects[blue][medium].w = ballsRects[blue][medium].h = 16;
	ballsRects[blue][little].x = 96;	ballsRects[blue][little].y = 48;	ballsRects[blue][little].w = ballsRects[blue][little].h = 8;

	ballsRects[green][huge].x = 0;		ballsRects[green][huge].y = 96;		ballsRects[green][huge].w = ballsRects[green][huge].h = 48;
	ballsRects[green][big].x = 48;		ballsRects[green][big].y = 96;		ballsRects[green][big].w = ballsRects[green][big].h = 32;
	ballsRects[green][medium].x = 80;	ballsRects[green][medium].y = 96;	ballsRects[green][medium].w = ballsRects[green][medium].h = 16;
	ballsRects[green][little].x = 96;	ballsRects[green][little].y = 96;	ballsRects[green][little].w = ballsRects[green][little].h = 8;

	ballsGraphics = App->textures->Load("./Image_Sources/Balls.png");
	if (ballsGraphics == NULL)
	{
		LOG("-------------------------Could not load balls Graphics.------------------------------")
	}
	
	return true;
}

update_status ModuleBalls::Update()
{

	p2List_item<Ball*>* pointer = ballsList.getFirst();
	p2List_item<Ball*>* pointer_next = ballsList.getFirst();

	while (pointer != NULL)
	{
		pointer_next = pointer->next;

		//If the baall has to be destroyed, we erase it
		if (pointer->data->Update(pauseBalls) == false)
		{
			std::cout << "-- Destroying ball --" << std::endl;
			//Ball subdivision
			if (pointer->data->type > 1)
			{
				Ball* newBall1 = new Ball(pointer->data, -1);
				Ball* newBall2 = new Ball(pointer->data, 1);

				ballsList.add(newBall1);
				ballsList.add(newBall2);
			}
			//	delete ball;	
			delete pointer->data;
			ballsList.del(pointer);

		}
		//Updating position and rendering
		else
		{
			CheckBricksColision();
			pointer->data->start_rect.x = pointer->data->position.x - pointer->data->radius;
			pointer->data->start_rect.y = pointer->data->position.y - pointer->data->radius;
			App->renderer->Blit(ballsGraphics, pointer->data->position.x, pointer->data->position.y, &ballsRects[red][pointer->data->type], pointer->data->radius, pointer->data->radius);
		}
		pointer = pointer_next;
	}
	return UPDATE_CONTINUE;
}

bool ModuleBalls::CleanUp()
{
	LOG("Cleanup Balls");
	p2List_item<Ball*>* pointer = ballsList.getFirst();
	p2List_item<Ball*>* pointer_next = ballsList.getFirst();
	while (pointer != NULL)
	{
		pointer_next = pointer->next;
		//Delete ball
		delete pointer->data;
		ballsList.del(pointer);
		pointer = pointer_next;
	}
	App->textures->Unload(ballsGraphics);
	return true;
}


//Other methods

void ModuleBalls::AddBall(int position_x, int position_y, int _type, int _direction)
{
	Ball* newBall = new Ball(position_x, position_y, _type, _direction);
	ballsList.add(newBall);
}

void ModuleBalls::CheckBricksColision()
{
	
	p2List_item<Ball*>* currentBall = ballsList.getFirst();
	int directionX;
	int directionY;
	int currentTileX;
	int currentTileY;
	bool collided;

	while (currentBall != NULL)
	{
		collided = false;
		if (currentBall->data->speed.x > 0) { directionX = 1; }
		else { directionX = -1; }
		if (currentBall->data->speed.y > 0) { directionY = 1; }
		else { directionY = -1; }

		currentTileX = currentBall->data->position.x / 8;
		currentTileY = currentBall->data->position.y / 8;

		switch (currentBall->data->type)
		{
		case huge:
		{
					for (int h = -3; h <= 3 && collided == false; h++)
					 {
						 if (App->backgroundPlay->debugMode == true)
						 {
							 SDL_Rect rect;
							 rect.x = (currentTileX + (3 * directionX)) * 8; rect.y = (currentTileY + h) * 8; rect.h = 8; rect.w = 8;
							 App->renderer->DrawQuad(rect, 255, 0, 0, 100);
						 }
						 if (App->maps->map[currentTileY + h][currentTileX + ( 3 * directionX )] == 1 && collided == false)
						 {
							 collided = CheckColision(currentTileX + 3 * directionX, currentTileY + h, currentBall->data);
						 }
					 }
					 for (int w = -3; w <= 3 && collided == false; w++)
					 {
						 if (App->backgroundPlay->debugMode == true)
						 {
							 SDL_Rect rect;
							 rect.x = (currentTileX + w) * 8; rect.y = (currentTileY + (3 * directionY)) * 8; rect.h = 8; rect.w = 8;
							 App->renderer->DrawQuad(rect, 255, 0, 0, 100);
						 }
						 if (App->maps->map[currentTileY + (3 * directionY)][currentTileX + w] == 1 && collided == false)
						 {
							 collided = CheckColision(currentTileX + 3 * directionX, currentTileY + w, currentBall->data);
						 }
					 }
					 if (App->backgroundPlay->debugMode == true)
					 {
						 SDL_Rect rect;
						 rect.x = (currentTileX + 2 * directionX) * 8; rect.y = (currentTileY + 2 * directionY) * 8; rect.h = 8; rect.w = 8;
						 App->renderer->DrawQuad(rect, 255, 0, 0, 100);
					 }
					 if (App->maps->map[currentTileY + 2 * directionY][currentTileX + 2 * directionX] == 1 && collided == false)
					 {
						 collided = CheckColision(currentTileX + 2 * directionX, currentTileY + 2 * directionY, currentBall->data);
					 }
					break;
		}
		case big:
		{
					for (int h = -2; h <= 2 && collided == false; h++)
					{
						if (App->backgroundPlay->debugMode == true)
						{
							SDL_Rect rect;
							rect.x = (currentTileX + 2 * directionX) * 8; rect.y = (currentTileY + h) * 8; rect.h = 8; rect.w = 8;
							App->renderer->DrawQuad(rect, 255, 0, 0, 100);
						}
						if (App->maps->map[currentTileY + h][currentTileX + 2 * directionX] == 1 && collided == false)
						{
							collided = CheckColision(currentTileX + 2 * directionX, currentTileY + h, currentBall->data);
						}
					}
					for (int w = -2; w <= 2 && collided == false; w++)
					{
						if (App->backgroundPlay->debugMode == true)
						{
							SDL_Rect rect;
							rect.x = (currentTileX + w) * 8; rect.y = (currentTileY + 2 * directionY) * 8; rect.h = 8; rect.w = 8;
							App->renderer->DrawQuad(rect, 255, 0, 0, 100);
						}
						if (App->maps->map[currentTileY + 2 * directionY][currentTileX + w] == 1 && collided == false)
						{
							collided = CheckColision(currentTileX + 2 * directionX, currentTileY + w, currentBall->data);
						}
					}
					if (App->backgroundPlay->debugMode == true)
					{
						SDL_Rect rect;
						rect.x = (currentTileX + 1 * directionX) * 8; rect.y = (currentTileY + 1 * directionY) * 8; rect.h = 8; rect.w = 8;
						App->renderer->DrawQuad(rect, 255, 0, 0, 100);
					}
					if (App->maps->map[currentTileY + 1 * directionY][currentTileX + 1 * directionX] == 1 && collided == false)
					{
						collided = CheckColision(currentTileX + 1 * directionX, currentTileY + 1 * directionY, currentBall->data);
					}
					break;
		}
		case medium:
		{
						for (int w = -1; w <= 1 && collided == false; w++)
					   {
						   for (int h = -1; h <= 1 && collided == false; h++)
						   {
							   if (App->backgroundPlay->debugMode == true)
							   {
								   SDL_Rect rect;
								   rect.x = (currentTileX + w) * 8; rect.y = (currentTileY + h) * 8; rect.h = 8; rect.w = 8;
								   App->renderer->DrawQuad(rect, 255, 0, 0, 100);
							   }
							   if (App->maps->map[currentTileY + h][currentTileX + w] == 1 && collided == false)
							   {
     								   collided = CheckColision(currentTileX + w, currentTileY + h, currentBall->data);
							   }
						   }
					   }
					   break;
		}
		case little:
		{
					   for (int w = 0; w <= 1; w++)
					   {
						   for (int h = 0; h <= 1; h++)
						   {
							   if (App->backgroundPlay->debugMode == true)
							   {
								   SDL_Rect rect;
								   rect.x = (currentTileX + (w*directionX)) * 8; rect.y = (currentTileY + (h*directionY)) * 8; rect.h = 8; rect.w = 8;
								   App->renderer->DrawQuad(rect, 255, 0, 0, 100);
							   }
							   if (App->maps->map[currentTileY + (h * directionY)][currentTileX + (w * directionX)] == 1 && collided == false)
							   {
								   collided = CheckColision(currentTileX + (w * directionX), currentTileY + (h * directionY), currentBall->data);
							   }
						   }
					   }
					   break;
		}

		}

		currentBall = currentBall->next;
	}
}

bool ModuleBalls::CheckColision(int tileX, int tileY, Ball* myBall)
{
	bool ret = false;
	if (tileY >= 25)
	{
		if (myBall->position.y + myBall->radius >= 25 * TILE)
		{
			myBall->speed.y = myBall->YBaseSpeed;
			myBall->position.y += myBall->speed.y;
			ret = true;
		}
	}
	else if (tileX <= 1 || tileX >= 47)
	{
		if (myBall->position.x - myBall->radius <= TILE || myBall->position.x + myBall->radius >= 47 * TILE)
		{
			myBall->speed.x *= -1;
			myBall->position.x += myBall->speed.x;
			ret = true;
		}
	}
	else
	{
		p2Point<float> points[4];
		points[0].x = tileX * 8;		points[0].y = tileY * 8;
		points[1].x = (tileX + 1) * 8;	points[1].y = tileY * 8;
		points[2].x = tileX * 8;		points[2].y = (tileY + 1) * 8;
		points[3].x = (tileX + 1) * 8;	points[3].y = (tileY + 1) * 8;

		for (int n = 0; n < 4; n++)
		{
			if (myBall->radius >= myBall->position.DistanceTo(points[n]))
			{
 				if (myBall->position.y < points[0].y)
				{
					myBall->speed.y *= -1;
					myBall->position.y += myBall->speed.y;
					ret = true;
				}
				else if (myBall->position.y > points[2].y)
				{
					myBall->speed.y *= -1;
					myBall->position.y += myBall->speed.y;
					ret = true;
				}
				else if (myBall->position.x < points[0].x)
				{
					myBall->speed.x *= -1;
					myBall->position.x += myBall->speed.x;
					ret = true;
				}
				else if (myBall->position.x > points[1].x)
				{
					myBall->speed.x *= -1;
					myBall->position.x += myBall->speed.x;
					ret = true;
				}
			}
		}
	}
	return ret;
}