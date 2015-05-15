
#include "Globals.h"
#include "Application.h"
#include "ModuleBalls.h"


#define GRAVITY 0.12f

////  CLASS BALL  //////////////////////////////////////

//Create a child ball
Ball::Ball(Ball* parent, int offsetDirection)
{
	position.y = parent->position.y;
	position.x = parent->position.x + (parent->offset * offsetDirection);
	type = parent->type - 1;
	color = parent->color;
	CreateBall(offsetDirection);
}

//Create the ball when initializing the level
Ball::Ball(int x, int y, int _type, int _color = red, int direction = 1)
{
	position.y = y;
	position.x = x;
	color = _color;
	type = _type;
	CreateBall(direction);
}

void Ball::CreateBall(int direction)
{
	switch (type)
	{
		case huge:
		{speed.y = -2.0f; speed.x = 1.6f * direction; radius = 24; YBaseSpeed = -5.7f; offset = 8; break; }
		case big:
		{speed.y = -2.5f; speed.x = 1.8f * direction; radius = 16; YBaseSpeed = -5.5f; offset = 8; break; }
		case medium:
		{speed.y = -2.0f; speed.x = 1.5f * direction; radius = 8; YBaseSpeed = -4.6f; offset = 4; break; }
		case little:
		{speed.y = -1.5f; speed.x = 1.4f * direction; radius = 4; YBaseSpeed = -3.0f; offset = 0; break; }
	}
	dead = false;
}


void Ball::Update(bool pause)
{
	if (speed.y > 7.0f) { speed.y = 7.0f; }
	if (pause == false)
	{
		position.x += speed.x;
		position.y += speed.y;
		speed.y += GRAVITY;
	}
	
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
	pauseBalls = false;
	
	return true;
}

update_status ModuleBalls::PreUpdate()
{
	p2List_item<Ball*>* pointer = ballsList.getFirst();
	p2List_item<Ball*>* pointer2 = pointer;

	while (pointer != NULL)
	{
		pointer2 = pointer->next;
		if (pointer->data->dead == true)
		{
			//	delete ball;	
			delete pointer->data;
			ballsList.del(pointer);
		}
		pointer = pointer2;
	}
	return UPDATE_CONTINUE;
}

update_status ModuleBalls::Update()
{

	p2List_item<Ball*>* pointer = ballsList.getFirst();

	CheckBoosts();

	while (pointer != NULL)
	{
		pointer->data->Update(pauseBalls);

		//If the ball has to be destroyed, we divide it
		if (pointer->data->dead == true)
		{
			LOG("-- Destroying ball --\n");
			//Ball subdivision
			if (pointer->data->type > little)
			{
				Ball* newBall1 = new Ball(pointer->data, -1);
				Ball* newBall2 = new Ball(pointer->data, 1);

				ballsList.add(newBall1);
				ballsList.add(newBall2);
			}
			App->particles->AddParticle(App->particles->explosion[pointer->data->color][pointer->data->type], pointer->data->position.x, pointer->data->position.y, 15, 15);
			if (rand() % 100 <= 25) { App->boosts->AddBoost(pointer->data->position.x, pointer->data->position.y); }
		}
		else
		{
			CheckBricksColision(pointer);
			App->renderer->Blit(ballsGraphics, pointer->data->position.x, pointer->data->position.y, &ballsRects[pointer->data->color][pointer->data->type], pointer->data->radius, pointer->data->radius);
			if (App->backgroundPlay->debugMode == true)
			{
				SDL_Rect myBallPos;
				myBallPos.h = myBallPos.w = 1;
				myBallPos.x = pointer->data->position.x;
				myBallPos.y = pointer->data->position.y;
				App->renderer->DrawQuad(myBallPos, 0, 255, 0, 255);
			}
		}
		pointer = pointer->next;
	}
	return UPDATE_CONTINUE;
}

bool ModuleBalls::CleanUp()
{
	LOG("Cleanup Balls");

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

void ModuleBalls::AddBall(int position_x, int position_y, int _type, int _color=red, int _direction=1)
{
	Ball* newBall = new Ball(position_x, position_y, _type, _color, _direction);
	ballsList.add(newBall);
}


void ModuleBalls::CheckBricksColision(p2List_item<Ball*>* currentBall)
{
	
	
	int directionX;
	int directionY;
	int currentTileX;
	int currentTileY;
	bool collided;
	SDL_Rect rect;

	
		collided = false;
		//Deciding which sides of the ball we'll check for colisions
		if (currentBall->data->speed.x >= 0) { directionX = 1; }
		else { directionX = -1; }
		if (currentBall->data->speed.y >= 0) { directionY = 1; }
		else { directionY = -1; }
		//Getting the current tile of the center of the ball
		currentTileX = currentBall->data->position.x / 8;
		currentTileY = currentBall->data->position.y / 8;

		//Comparing the necessary tiles to see if there's any colision. To see the shape of each ball, enable "Debug Mode"
		//When it may collide, call "Check collision". If it does, this ball will stop looking for anymore collisions with the surroundings.

		if ((currentBall->data->position.y + currentBall->data->radius + 1) / TILE >= 25)
		{
 			currentBall->data->speed.y = currentBall->data->YBaseSpeed;
			currentBall->data->position.y = 25 * TILE - currentBall->data->radius;
			collided = true;
		}
		
		else 
		{
			switch (currentBall->data->type)
			{


				//Huge Ball 
			case huge:
			{
				for (int h = -2; h <= 2 && collided == false; h++)
				{
#pragma region RenderDebugTiles
							 //This piece of code are just for rendering the squares in "debug mode", not used in the actual game.
							 //VVVVV
							 if (App->backgroundPlay->debugMode == true)
							 {
								 rect.x = (currentTileX + (2 * directionX)) * 8; rect.y = (currentTileY + h) * 8; rect.h = 8; rect.w = 8;
								 App->renderer->DrawQuad(rect, 255, 0, 0, 100);
							 }
							 //AAAAA
#pragma endregion
					if (App->maps->map[currentTileY + h][currentTileX + (2 * directionX)] == 1 && collided == false)
					{
						collided = CheckColision(currentTileX + (2 * directionX), currentTileY + h, currentBall->data);
#pragma region RenderDebugTiles
								 if (App->backgroundPlay->debugMode == true){ App->renderer->DrawQuad(rect, 0, 0, 255, 150); }
#pragma endregion
					}
				}
				for (int w = -2; w <= 2 && collided == false; w++)
				{
#pragma region RenderDebugTiles
							 //VVVVV

							 if (App->backgroundPlay->debugMode == true)
							 {
								 rect.x = (currentTileX + w) * 8; rect.y = (currentTileY + (2 * directionY)) * 8; rect.h = 8; rect.w = 8;
								 App->renderer->DrawQuad(rect, 255, 0, 0, 100);
							 }
							 //AAAAA
#pragma endregion
					if (App->maps->map[currentTileY + (2 * directionY)][currentTileX + w] == 1 && collided == false)
					{
						collided = CheckColision(currentTileX + w, currentTileY + (2 * directionY), currentBall->data);
#pragma region RenderDebugTiles
								 if (App->backgroundPlay->debugMode == true){ App->renderer->DrawQuad(rect, 0, 0, 255, 150); }
#pragma endregion
					}
				}
				for (int h = -2; h <= 2 && collided == false; h++)
				{
#pragma region RenderDebugTiles
					//This piece of code are just for rendering the squares in "debug mode", not used in the actual game.
					//VVVVV
					if (App->backgroundPlay->debugMode == true)
					{
						rect.x = (currentTileX + (3 * directionX)) * 8; rect.y = (currentTileY + h) * 8; rect.h = 8; rect.w = 8;
						App->renderer->DrawQuad(rect, 255, 0, 0, 100);
					}
					//AAAAA
#pragma endregion
					if (App->maps->map[currentTileY + h][currentTileX + (3 * directionX)] == 1 && collided == false)
					{
						collided = CheckColision(currentTileX + (3 * directionX), currentTileY + h, currentBall->data);
#pragma region RenderDebugTiles
						if (App->backgroundPlay->debugMode == true){ App->renderer->DrawQuad(rect, 0, 0, 255, 150); }
#pragma endregion
					}
				}
				for (int w = -2; w <= 2 && collided == false; w++)
				{
#pragma region RenderDebugTiles
					//VVVVV

					if (App->backgroundPlay->debugMode == true)
					{
						rect.x = (currentTileX + w) * 8; rect.y = (currentTileY + (3 * directionY)) * 8; rect.h = 8; rect.w = 8;
						App->renderer->DrawQuad(rect, 255, 0, 0, 100);
					}
					//AAAAA
#pragma endregion
					if (App->maps->map[currentTileY + (3 * directionY)][currentTileX + w] == 1 && collided == false)
					{
						collided = CheckColision(currentTileX + w, currentTileY + (3 * directionY), currentBall->data);
#pragma region RenderDebugTiles
						if (App->backgroundPlay->debugMode == true){ App->renderer->DrawQuad(rect, 0, 0, 255, 150); }
#pragma endregion
					}
				}
				break;
			}//End of case: huge


			case big:
			{
						for (int h = -1; h <= 1 && collided == false; h++)
						{
#pragma region RenderDebugTiles
							//This piece of code are just for rendering the squares in "debug mode", not used in the actual game.
							//VVVVV
							if (App->backgroundPlay->debugMode == true)
							{
								rect.x = (currentTileX + (2 * directionX)) * 8; rect.y = (currentTileY + h) * 8; rect.h = 8; rect.w = 8;
								App->renderer->DrawQuad(rect, 255, 0, 0, 100);
							}
							//AAAAA
#pragma endregion
							if (App->maps->map[currentTileY + h][currentTileX + (2 * directionX)] == 1 && collided == false)
							{
								collided = CheckColision(currentTileX + (2 * directionX), currentTileY + h, currentBall->data);
#pragma region RenderDebugTiles
								if (App->backgroundPlay->debugMode == true){ App->renderer->DrawQuad(rect, 0, 0, 255, 150); }
#pragma endregion
							}
						}
						for (int w = -1; w <= 1 && collided == false; w++)
						{
#pragma region RenderDebugTiles
							//VVVVV

							if (App->backgroundPlay->debugMode == true)
							{
								rect.x = (currentTileX + w) * 8; rect.y = (currentTileY + (2 * directionY)) * 8; rect.h = 8; rect.w = 8;
								App->renderer->DrawQuad(rect, 255, 0, 0, 100);
							}
							//AAAAA
#pragma endregion
							if (App->maps->map[currentTileY + (2 * directionY)][currentTileX + w] == 1 && collided == false)
							{
								collided = CheckColision(currentTileX + w, currentTileY + (2 * directionY), currentBall->data);
#pragma region RenderDebugTiles
								if (App->backgroundPlay->debugMode == true){ App->renderer->DrawQuad(rect, 0, 0, 255, 150); }
#pragma endregion
							}
						}
						for (int h = -1; h <= 1 && collided == false; h++)
						{
#pragma region RenderDebugTiles
							//This piece of code are just for rendering the squares in "debug mode", not used in the actual game.
							//VVVVV
							if (App->backgroundPlay->debugMode == true)
							{
								rect.x = (currentTileX + (1 * directionX)) * 8; rect.y = (currentTileY + h) * 8; rect.h = 8; rect.w = 8;
								App->renderer->DrawQuad(rect, 255, 0, 0, 100);
							}
							//AAAAA
#pragma endregion
							if (App->maps->map[currentTileY + h][currentTileX + (1 * directionX)] == 1 && collided == false)
							{
								collided = CheckColision(currentTileX + (1 * directionX), currentTileY + h, currentBall->data);
#pragma region RenderDebugTiles
								if (App->backgroundPlay->debugMode == true){ App->renderer->DrawQuad(rect, 0, 0, 255, 150); }
#pragma endregion
							}
						}
						for (int w = -1; w <= 1 && collided == false; w++)
						{
#pragma region RenderDebugTiles
							//VVVVV

							if (App->backgroundPlay->debugMode == true)
							{
								rect.x = (currentTileX + w) * 8; rect.y = (currentTileY + (1 * directionY)) * 8; rect.h = 8; rect.w = 8;
								App->renderer->DrawQuad(rect, 255, 0, 0, 100);
							}
							//AAAAA
#pragma endregion
							if (App->maps->map[currentTileY + (1 * directionY)][currentTileX + w] == 1 && collided == false)
							{
								collided = CheckColision(currentTileX + w, currentTileY + (1 * directionY), currentBall->data);
#pragma region RenderDebugTiles
								if (App->backgroundPlay->debugMode == true){ App->renderer->DrawQuad(rect, 0, 0, 255, 150); }
#pragma endregion
							}
						}
						break;
			}//End of case: big


			case medium:
			{
				for (int h = -1; h <= 1 && collided == false; h+=2)
				{

#pragma region RenderDebugTiles
					//VVVVV
					if (App->backgroundPlay->debugMode == true)
					{
						rect.x = (currentTileX) * 8; rect.y = (currentTileY + h) * 8; rect.h = 8; rect.w = 8;
						App->renderer->DrawQuad(rect, 255, 0, 0, 100);
					}
					//AAAAA
#pragma endregion
					if (App->maps->map[currentTileY + h][currentTileX] == 1 && collided == false)
					{
						collided = CheckColision(currentTileX, currentTileY + h, currentBall->data);
#pragma region RenderDebugTiles
						if (App->backgroundPlay->debugMode == true){ App->renderer->DrawQuad(rect, 0, 0, 255, 150); }
#pragma endregion
					}

				}
				for (int w = -1; w <= 1 && collided == false; w+=2)
				{

#pragma region RenderDebugTiles
					//VVVVV
					if (App->backgroundPlay->debugMode == true)
					{
						rect.x = (currentTileX + w)* 8; rect.y = (currentTileY) * 8; rect.h = 8; rect.w = 8;
						App->renderer->DrawQuad(rect, 255, 0, 0, 100);
					}
					//AAAAA
#pragma endregion
					if (App->maps->map[currentTileY][currentTileX + w] == 1 && collided == false)
					{
						collided = CheckColision(currentTileX + w, currentTileY, currentBall->data);
#pragma region RenderDebugTiles
						if (App->backgroundPlay->debugMode == true){ App->renderer->DrawQuad(rect, 0, 0, 255, 150); }
#pragma endregion
					}

				}
				for (int w = -1; w <= 1 && collided == false; w+=2)
				{
					for (int h = -1; h <= 1 && collided == false; h += 2)
					{
#pragma region RenderDebugTiles
						//VVVVV
						if (App->backgroundPlay->debugMode == true)
						{
							rect.x = (currentTileX + w) * 8; rect.y = (currentTileY + h) * 8; rect.h = 8; rect.w = 8;
							App->renderer->DrawQuad(rect, 255, 0, 0, 100);
						}
						//AAAAA
#pragma endregion
						if (App->maps->map[currentTileY + h][currentTileX + w] == 1 && collided == false)
						{
							collided = CheckColision(currentTileX + w, currentTileY + h, currentBall->data);
#pragma region RenderDebugTiles
							if (App->backgroundPlay->debugMode == true){ App->renderer->DrawQuad(rect, 0, 0, 255, 150); }
#pragma endregion
						}
					}
				}
				break;
			}//End of case: medium


			case little:
			{
						   for (int h = 0; h <= 1 && collided == false; h++)
						   {
							   for (int w = 0; w <= 1 && collided == false; w++)
							   {

#pragma region RenderDebugTiles
								   //VVVVV
								   if (App->backgroundPlay->debugMode == true)
								   {
									   rect.x = (currentTileX + w * directionX) * 8; rect.y = (currentTileY + h * directionY) * 8; rect.h = 8; rect.w = 8;
									   App->renderer->DrawQuad(rect, 255, 0, 0, 100);
								   }
								   //AAAAA
#pragma endregion
								   if (App->maps->map[currentTileY + h * directionY][currentTileX + w * directionX] == 1 && collided == false)
								   {
									   collided = CheckColision(currentTileX + w * directionX, currentTileY + h * directionY, currentBall->data);
#pragma region RenderDebugTiles
									   if (App->backgroundPlay->debugMode == true){ App->renderer->DrawQuad(rect, 0, 0, 255, 150); }
#pragma endregion
								   }

							   }
						   }
			}//End of case: little


			}//End of switch (type)
		}
		
}

bool ModuleBalls::CheckColision(int tileX, int tileY, Ball* myBall)
{
	bool ret = false;
	//Now, bricks. Checking for all 4 vertex of the tile to see if it collides with any of them.
	float distance[4];
	p2Point<float> points[4];
	points[0].x = tileX * 8;		points[0].y = tileY * 8;
	points[1].x = (tileX + 1) * 8;	points[1].y = tileY * 8;
	points[2].x = tileX * 8;		points[2].y = (tileY + 1) * 8;
	points[3].x = (tileX + 1) * 8;	points[3].y = (tileY + 1) * 8;

	for (int n = 0; n < 4 && App->backgroundPlay->debugMode == true; n++)
	{
		SDL_Rect myBallPos;
		myBallPos.h = myBallPos.w = 1;
		myBallPos.x = points[n].x;
		myBallPos.y = points[n].y;
		App->renderer->DrawQuad(myBallPos, 0, 255, 0, 255);
	}

	for (int n = 0; n < 4; n++)
	{
		distance[n] = myBall->position.DistanceTo(points[n]);
	}

	for (int n = 0; n < 4 && ret == false; n++)
	{
		//Changing the speed of the ball dependin on the 2 closest vertex to the ball
		if (distance[n] <= myBall->radius + 3)
		{
			distance[n] = 1000000;
			float closest = MIN(distance[0], MIN(distance[1], MIN(distance[2], distance[3])));
	//Collided at bottom
			if ((n == 0 || n == 1) && ret == false && myBall->speed.y >= 0)
			{
				if (closest + 0.2f > distance[0] || closest + 0.2f > distance[1])
				{
					myBall->speed.y *= -1;
					myBall->speed.y += 0.1f;
					myBall->position.y = points[0].y - myBall->radius;
					ret = true;
				}
			}
	//Collided at top
			if ((n == 2 || n == 3) && ret == false && myBall->speed.y <= 0)
			{
				if (closest + 0.2f > distance[2] || closest + 0.2f > distance[3])
				{
					myBall->speed.y *= -1;
					myBall->position.y = points[2].y + myBall->radius;
					ret = true;
				}
			}

	//Collided right
			if ((n == 0 || n == 2) && ret == false && myBall->speed.x >= 0)
			{
				if (closest + 0.2f > distance[0] || closest + 0.2f > distance[2])
				{
					myBall->speed.x *= -1;
					myBall->position.x = points[0].x - myBall->radius;
					ret = true;
				}
			}
	//Collided at left
			if ((n == 1 || n == 3) && ret == false && myBall->speed.x <= 0)
			{
				if (closest + 0.2f > distance[1] || closest + 0.2f > distance[3])
				{
					myBall->speed.x *= -1;
					myBall->position.x = points[1].x + myBall->radius;
					ret = true;
				}
			}

		}
	}
	return ret;
}

void ModuleBalls::CheckBoosts()
{
	if (pauseBoost == true)
	{
		pauseCounter++;
		if (pauseCounter >= PAUSE_BOOST_TIME)
		{
			pauseBalls = false;
			pauseBoost = false;
			pauseCounter = 0;
		}
	}
	if (bombBoost == true)
	{
		bombCounter++;
		if (bombCounter == 1 || bombCounter == 30 || bombCounter == 60)
		{
			Bomb();
		}
		if (bombCounter >= 90)
		{
			Bomb();
			bombBoost = false;
		}
	}
}

void ModuleBalls::BombBoost()
{
	bombBoost = true;
	bombCounter = 0;
}

void ModuleBalls::PauseBoost()
{
	pauseCounter = 0;
	pauseBalls = true;
	pauseBoost = true;
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