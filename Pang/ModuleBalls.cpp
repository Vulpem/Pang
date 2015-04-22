
#include "Globals.h"
#include "Application.h"
#include "ModuleBalls.h"

#include <iostream>

#define GRAVITY 0.2

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
			speed.y += GRAVITY;
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

	/*
	p2List_item<Ball*>* pointer = ballsList.getFirst();
	p2List_item<Ball*>* pointer_next = ballsList.getFirst();
	while (pointer != NULL)
	{
		pointer_next = pointer->next;
		//Delete ball
		delete pointer->data;
		ballsList.del(pointer);
		pointer = pointer_next;
	}*/

	p2List_item<Ball*>* pointer = ballsList.getFirst();
	while (pointer != NULL)
	{
		delete pointer->data;
		pointer = pointer->next;
	}

	ballsList.clear();

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
	SDL_Rect rect;

	while (currentBall != NULL)
	{
		collided = false;
		//Deciding which sides of the ball we'll check for colisions
		if (currentBall->data->speed.x > 0) { directionX = 1; }
		else { directionX = -1; }
		if (currentBall->data->speed.y > 0) { directionY = 1; }
		else { directionY = -1; }
		//Getting the current tile of the center of the ball
		currentTileX = currentBall->data->position.x / 8;
		currentTileY = currentBall->data->position.y / 8;

		//Comparing the necessary tiles to see if there's any colision. To see the shape of each ball, enable "Debug Mode"
		//When it may collide, call "Check collision". If it does, this ball will stop looking for anymore collisions with the surroundings.
		switch (currentBall->data->type)
		{
			//Huge Ball 
		case huge:
		{
					for (int h = -2; h <= 2 && collided == false; h++)
					 {
						 //This piece of code in between the "VVV" and the "AAA" and all the following similar to this one are just for rendering the squares in "debug mode", not used in the actual game.
						 //VVVVV
						 if (App->backgroundPlay->debugMode == true)
						 {
							 rect.x = (currentTileX + (3 * directionX)) * 8; rect.y = (currentTileY + h) * 8; rect.h = 8; rect.w = 8;
							 App->renderer->DrawQuad(rect, 255, 0, 0, 100);
						 }
						 //AAAAA
						 if (App->maps->map[currentTileY + h][currentTileX + ( 3 * directionX )] == 1 && collided == false)
						 {
							 collided = CheckColision(currentTileX + 3 * directionX, currentTileY + h, currentBall->data);
							 if (App->backgroundPlay->debugMode == true){ App->renderer->DrawQuad(rect, 0, 0, 255, 150); }
						 }
					 }
					 for (int w = -2; w <= 2 && collided == false; w++)
					 {
						 //VVVVV
						 if (App->backgroundPlay->debugMode == true)
						 {
							 rect.x = (currentTileX + w) * 8; rect.y = (currentTileY + (3 * directionY)) * 8; rect.h = 8; rect.w = 8;
							 App->renderer->DrawQuad(rect, 255, 0, 0, 100);
						 }
						 //AAAAA
						 if (App->maps->map[currentTileY + (3 * directionY)][currentTileX + w] == 1 && collided == false)
						 {
							 collided = CheckColision(currentTileX + 3 * directionX, currentTileY + w, currentBall->data);
							 if (App->backgroundPlay->debugMode == true){ App->renderer->DrawQuad(rect, 0, 0, 255, 150); }
						 }
					 }
					 //VVVVV
					 if (App->backgroundPlay->debugMode == true)
					 {
						 rect.x = (currentTileX + 2 * directionX) * 8; rect.y = (currentTileY + 2 * directionY) * 8; rect.h = 8; rect.w = 8;
						 App->renderer->DrawQuad(rect, 255, 0, 0, 100);
					 }
					 //AAAAA
					 if (App->maps->map[currentTileY + 2 * directionY][currentTileX + 2 * directionX] == 1 && collided == false)
					 {
						 collided = CheckColision(currentTileX + 2 * directionX, currentTileY + 2 * directionY, currentBall->data);
						 if (App->backgroundPlay->debugMode == true){ App->renderer->DrawQuad(rect, 0, 0, 255, 150); }
					 }
					 //VVVVV
					 if (App->backgroundPlay->debugMode == true)
					 {
						 rect.x = (currentTileX - 3 * directionX) * 8; rect.y = (currentTileY + 2 * directionY) * 8; rect.h = 8; rect.w = 8;
						 App->renderer->DrawQuad(rect, 255, 0, 0, 100);
					 }
					 //AAAAA
					 if (App->maps->map[currentTileY + 2 * directionY][currentTileX - 3 * directionX] == 1 && collided == false)
					 {
						 collided = CheckColision(currentTileX - 3 * directionX, currentTileY + 2 * directionY, currentBall->data);
						 if (App->backgroundPlay->debugMode == true){ App->renderer->DrawQuad(rect, 0, 0, 255, 150); }
					 }
					 if (App->backgroundPlay->debugMode == true)
					 {
						 rect.x = (currentTileX + 2 * directionX) * 8; rect.y = (currentTileY - 3 * directionY) * 8; rect.h = 8; rect.w = 8;
						 App->renderer->DrawQuad(rect, 255, 0, 0, 100);
					 }
					 //AAAAA
					 if (App->maps->map[currentTileY - 3 * directionY][currentTileX + 2 * directionX] == 1 && collided == false)
					 {
						 collided = CheckColision(currentTileX + 2 * directionX, currentTileY - 3 * directionY, currentBall->data);
						 if (App->backgroundPlay->debugMode == true){ App->renderer->DrawQuad(rect, 0, 0, 255, 150); }
					 }
					break;
		}
		case big:
		{
					for (int h = -2; h <= 2 && collided == false; h++)
					{
						//VVVVV
						if (App->backgroundPlay->debugMode == true)
						{
							rect.x = (currentTileX + 2 * directionX) * 8; rect.y = (currentTileY + h) * 8; rect.h = 8; rect.w = 8;
							App->renderer->DrawQuad(rect, 255, 0, 0, 100);
						}
						//AAAAA
						if (App->maps->map[currentTileY + h][currentTileX + 2 * directionX] == 1 && collided == false)
						{
							collided = CheckColision(currentTileX + 2 * directionX, currentTileY + h, currentBall->data);
							if (App->backgroundPlay->debugMode == true){ App->renderer->DrawQuad(rect, 0, 0, 255, 150); }
						}
					}
					for (int w = -2; w <= 2 && collided == false; w++)
					{
						//VVVVV
						if (App->backgroundPlay->debugMode == true)
						{
							rect.x = (currentTileX + w) * 8; rect.y = (currentTileY + 2 * directionY) * 8; rect.h = 8; rect.w = 8;
							App->renderer->DrawQuad(rect, 255, 0, 0, 100);
						}
						//AAAAA
						if (App->maps->map[currentTileY + 2 * directionY][currentTileX + w] == 1 && collided == false)
						{
							collided = CheckColision(currentTileX + 2 * directionX, currentTileY + w, currentBall->data);
							if (App->backgroundPlay->debugMode == true){ App->renderer->DrawQuad(rect, 0, 0, 255, 150); }
						}
					}
					//VVVVV
					if (App->backgroundPlay->debugMode == true)
					{
						rect.x = (currentTileX + 1 * directionX) * 8; rect.y = (currentTileY + 1 * directionY) * 8; rect.h = 8; rect.w = 8;
						App->renderer->DrawQuad(rect, 255, 0, 0, 100);
					}
					//AAAAA
					if (App->maps->map[currentTileY + 1 * directionY][currentTileX + 1 * directionX] == 1 && collided == false)
					{
						collided = CheckColision(currentTileX + 1 * directionX, currentTileY + 1 * directionY, currentBall->data);
						if (App->backgroundPlay->debugMode == true){ App->renderer->DrawQuad(rect, 0, 0, 255, 150); }
					}
					break;
		}
		case medium:
		{
						for (int h = -1; h <= 1 && collided == false; h++)
						{
							//VVVVV
							if (App->backgroundPlay->debugMode == true)
							{
								rect.x = (currentTileX + 1 * directionX) * 8; rect.y = (currentTileY + h) * 8; rect.h = 8; rect.w = 8;
								App->renderer->DrawQuad(rect, 255, 0, 0, 100);
							}
							//AAAAA
							if (App->maps->map[currentTileY + h][currentTileX + 1 * directionX] == 1 && collided == false)
							{
								collided = CheckColision(currentTileX + 1 * directionX, currentTileY + h, currentBall->data);
								if (App->backgroundPlay->debugMode == true){ App->renderer->DrawQuad(rect, 0, 0, 255, 150); }
							}
						}
						for (int w = -1; w <= 1 && collided == false; w++)
						{
							//VVVVV
							if (App->backgroundPlay->debugMode == true)
							{
								rect.x = (currentTileX + w) * 8; rect.y = (currentTileY + 1 * directionY) * 8; rect.h = 8; rect.w = 8;
								App->renderer->DrawQuad(rect, 255, 0, 0, 100);
							}
							//AAAAA
							if (App->maps->map[currentTileY + 1 * directionY][currentTileX + w] == 1 && collided == false)
							{
								collided = CheckColision(currentTileX + 1 * directionX, currentTileY + w, currentBall->data);
								if (App->backgroundPlay->debugMode == true){ App->renderer->DrawQuad(rect, 0, 0, 255, 150); }
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
							   //VVVVV
							   if (App->backgroundPlay->debugMode == true)
							   {
								   rect.x = (currentTileX + (w*directionX)) * 8; rect.y = (currentTileY + (h*directionY)) * 8; rect.h = 8; rect.w = 8;
								   App->renderer->DrawQuad(rect, 255, 0, 0, 100);
							   }
							   //AAAAA
							   if (App->maps->map[currentTileY + (h * directionY)][currentTileX + (w * directionX)] == 1 && collided == false)
							   {
								   collided = CheckColision(currentTileX + (w * directionX), currentTileY + (h * directionY), currentBall->data);
								   if (App->backgroundPlay->debugMode == true){ App->renderer->DrawQuad(rect, 0, 0, 255, 150); }
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
	//First of all, checking if the collision was casued by the floor, and if it really collides
	if (tileY >= 25)
	{
		if (myBall->position.y + myBall->radius >= 25 * TILE)
		{
			myBall->speed.y = myBall->YBaseSpeed;
			myBall->position.y += myBall->speed.y;
			ret = true;
		}
	}
	//Then, both walls.
	else if (tileX <= 1 || tileX >= 47)
	{
		if (myBall->position.x - myBall->radius <= TILE || myBall->position.x + myBall->radius >= 47 * TILE)
		{
			myBall->speed.x *= -1;
			myBall->position.x += myBall->speed.x;
			ret = true;
		}
	}
	//Now, bricks. Checking for all 4 vertex of the tile to see if it collides with any of them.
	else
	{
		p2Point<float> points[4];
		points[0].x = tileX * 8;		points[0].y = tileY * 8;
		points[1].x = (tileX + 1) * 8;	points[1].y = tileY * 8;
		points[2].x = tileX * 8;		points[2].y = (tileY + 1) * 8;
		points[3].x = (tileX + 1) * 8;	points[3].y = (tileY + 1) * 8;

		for (int n = 0; n < 4; n++)
		{
			//Comparing the distance from the center of the ball to the current vertex
			if (myBall->radius >= myBall->position.DistanceTo(points[n]))
			{
				//If it does collide, detecting from which side it collides, to change speed accordingly
				if (myBall->position.y < points[0].y || myBall->position.y > points[2].y)
				{
					myBall->speed.y *= -1;
					myBall->speed.y += GRAVITY*2;
					myBall->position.y += myBall->speed.y;
					ret = true;
				}
				else
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

void ModuleBalls::Bomb()
{
	p2List_item<Ball*>* currentBall = ballsList.getFirst();
	while (currentBall != NULL)
	{
		if (currentBall->data->type != little)
		{
			currentBall->data->dead = true;
		}
		currentBall = currentBall->next;
	}
}