#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleGun.h"

#include <iostream>
// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	graphics = NULL;

	//////////////
	//Animations//
	//////////////

	// idle animation
	idle.frames.PushBack({ 10, 112, 32, 32 });

	// idle2 animation
	idle2.frames.PushBack({ 14, 179, 28, 32 });
	// forward animation
	forward.frames.PushBack({ 10, 2, 32, 32 });
	forward.frames.PushBack({ 44, 2, 32, 32 });
	forward.frames.PushBack({ 78, 2, 32, 32 });
	forward.frames.PushBack({ 112, 2, 32, 32 });
	forward.frames.PushBack({ 10, 2, 32, 32 });
	forward.frames.PushBack({ 44, 2, 32, 32 });
	forward.frames.PushBack({ 78, 2, 32, 32 });
	forward.frames.PushBack({ 112, 2, 32, 32 });
	forward.frames.PushBack({ 146, 2, 32, 32 });
	forward.speed = 0.32f;
	
	// backward animation
	backward.frames.PushBack({ 14, 146, 28, 32 });
	backward.frames.PushBack({ 48, 146, 28, 32 });
	backward.frames.PushBack({ 82, 146, 28, 32 });
	backward.frames.PushBack({ 116, 146, 28, 32 });
	backward.frames.PushBack({ 14, 146, 28, 32 });
	backward.frames.PushBack({ 48, 146, 28, 32 });
	backward.frames.PushBack({ 82, 146, 28, 32 });
	backward.frames.PushBack({ 116, 146, 28, 32 });
	backward.frames.PushBack({ 150, 146, 28, 32 });
	backward.speed = 0.32f;


	// shot animation
	shot.frames.PushBack({ 44, 112, 32, 32 });

	// shot2 animation
	shot2.frames.PushBack({ 44, 179, 32, 32 });

	// climb animation
	climb.frames.PushBack({ 80, 36, 30, 32 });
	climb.frames.PushBack({ 47, 77, 30, 32 });
	climb.frames.PushBack({ 114, 36, 30, 32 });
	climb.frames.PushBack({ 47, 77, 30, 32 });

	climb.speed = 0.16f;

	endclimb.frames.PushBack({ 113, 78, 32, 32 });
}

ModulePlayer::~ModulePlayer()
{}


bool ModulePlayer::Start()
{
	LOG("--Initializing player");
	bool ret = true;
	graphics = App->textures->Load("./Image_Sources/Player.png");
	if (graphics == NULL)
	{
		LOG("------------------Could not load player graphics----------------------");
	}
	playerState = standing;
	position.x = TILE;
	position.y = 9 * TILE;
	ladderAlign = false;
	dead = false;

	return ret;
}


update_status ModulePlayer::Update()
{
	LOG("--Updating Player")
	IsFalling();
	Movement();
	Shoot();
	Climb();
	Fall();

	if (current_animation != NULL)
	{
		SDL_Rect r = current_animation->GetCurrentFrame();
		App->renderer->Blit(graphics, position.x - 2, position.y, &r);
	}
	CheckBallCollision();
	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) 
	{
		LOG("Changed undying mode\n" );
		undying = !undying;
	}
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		LOG("Pause\n");
		App->balls->pauseBalls = !App->balls->pauseBalls;
	}
	if (App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
	{
		LOG("Bomb \n" );
		App->balls->Bomb();
	}
	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		LOG( "NBalls: %i \n", App->balls->ballsList.count() );
	}
	if (App->backgroundPlay->debugMode == true)
	{
		/*
		Player bounding box
		*/
		SDL_Rect boundingBox;
		boundingBox.x = position.x + 4;
		boundingBox.y = position.y + 5;
		boundingBox.w = 16;
		boundingBox.h = 27;
		App->renderer->DrawQuad(boundingBox, 0, 200, 0, 150);
	}
	
	return UPDATE_CONTINUE;
}

bool ModulePlayer::CleanUp()
{
	LOG("--Cleanup Player");

	App->textures->Unload(graphics);

	return true;
}


//////  Player Methods //////


void ModulePlayer::IsFalling()
{
	if ((App->maps->map[position.y / 8 + 4][(position.x + 6) / 8] == 0 && App->maps->map[position.y / 8 + 4][(position.x + 17) / 8] == 0 || MiddleLadder()) && playerState != climbing)
	{
		playerState = falling;
	}
}

bool ModulePlayer::LadderFall()
{
	int counter = 0;
	for (int w = 0; w < 32; w++)
	{
		if (App->maps->map[(position.y + 2)/ 8 + 4][(position.x + w) / 8] == 2 && App->maps->map[(position.y + 2)/ 8 + 3][(position.x + w)/ 8] != 2)
		{
			counter++;
		}
		else
			counter = 0;
		if (counter >= 8)
		{
			LOG("Falling into ladder\n");
			return true;
		}

	}
	return false;
}

bool ModulePlayer::MiddleLadder()
{
	int counter = 0;
	for (int w = 0; w < 3; w++)
	{
		if (App->maps->map[position.y / 8 + 4][position.x / 8 + w] == 2 && App->maps->map[position.y / 8 + 3][position.x / 8 + w] == 2)
		{
			counter++;
		}
	}
	if (counter >= 2)
		return true;
	else
		return false;
}


void ModulePlayer::Movement()
{
	if (playerState != climbing)
	{
		if (movementDirection == 1)
			current_animation = &idle;
		if (movementDirection == -1)
			current_animation = &idle2;

		//Move right
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT && App->maps->map[position.y / 8][(position.x + 25) / 8] != 1)
			{
				current_animation = &forward;
				position.x += speed;
				movementDirection = 1;
			}

		}

		//Move left
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT && App->maps->map[position.y / 8][(position.x - 1) / 8] != 1)
			{
				current_animation = &backward;
				position.x -= speed;
				movementDirection = -1;
			}

		}
	}
}

void ModulePlayer::Shoot()
{
	if (App->gun->shootAvailable == true)
	{
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			p2Point<int> offset;
			offset.y = 32;
			offset.x = 11.5 + (2.5 * movementDirection);
			if (playerState == climbing)
			{
				
			}
			if (playerState != climbing)
			{
				if (movementDirection == 1)
				{
					current_animation = &shot;
				}

				else if (movementDirection == -1)
				{
					current_animation = &shot2;
				}
			}
			App->gun->Shoot((position + offset));

		}
	}
}

void ModulePlayer::Climb()
{
	if (playerState != falling)
	{
	if (playerState == climbing && App->input->GetKey(SDL_SCANCODE_W) != KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_S) != KEY_REPEAT)
		climb.speed = 0.0f;
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			climb.speed = 0.0f;
		}
		else
		{
			if (playerState == climbing && LadderUpEnds())
			{
				LOG("LadderUpEnds\n" );
				current_animation = &endclimb;
				playerState = standing;
				ladderAlign = false;
			}
		
			else if (CanClimbUp())
			{
				AlignLadder(1);
				climb.speed = 0.16f;
				playerState = climbing;
				current_animation = &climb;
				position.y-=2;
			}
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			climb.speed = 0.0f;
		}
		else
		{
			if (playerState == climbing && LadderDownEnds())
			{
				LOG("LadderDownEnds\n");
				current_animation = &endclimb;
				playerState = standing;
				ladderAlign = false;
			}

			else if (CanClimbDown())
			{
				AlignLadder(-1);
				climb.speed = 0.16f;
				playerState = climbing;
				current_animation = &climb;
				position.y+=2;
			}
		}
	}
	}


}

void ModulePlayer::Fall()
{
	if (playerState == falling)
	{
		speed = 1;
		fallCounter++;
		position.y += 2;

		if (fallCounter > 8)
		{
			position.y += 2;
		}
		//Arreglar perque caigui be
		if ((App->maps->map[position.y / 8 + 4][(position.x + 6) / 8] != 0 && App->maps->map[position.y / 8 + 4][(position.x + 17) / 8] != 0) || LadderFall())
		{
			//In case its a ladder
			fallCounter = 0;
			playerState = standing;
			speed = 2;			
			LOG("SpeedChange\n");
		}
	}
}

void ModulePlayer::Kill()
{
	LOG("Player has died\n");

		App->balls->pauseBalls = true;
		App->fade->FadeToBlack(App->backgroundPlay, App->backgroundIntro, 3.0f);
	
}

void ModulePlayer::CheckBallCollision()
{
	p2List_item<Ball*>* tmp = App->balls->ballsList.getFirst();
	while (tmp != NULL && !dead)
	{
		if ((tmp->data->position.y + tmp->data->radius >= position.y + 5) &&
			(tmp->data->position.y - tmp->data->radius <= position.y + 27) &&
			((tmp->data->position.x + tmp->data->radius) > position.x + 4) &&
			(tmp->data->position.x - tmp->data->radius) < position.x + 20)
		{
			if (undying == false)
			{
				Kill();
				dead = true;
			}
		}
		tmp = tmp->next;
	}
}

bool ModulePlayer::LadderUpEnds()
{
	for (int w = 0; w < 3; w++)
	{
	if (App->maps->map[(position.y + 31) / 8][(position.x)/ 8 + w] == 0)
		return true;
	}

	return false;
}

bool ModulePlayer::LadderDownEnds()
{
	for (int w = 0; w < 3; w++)
	{
		if (App->maps->map[(position.y + 32) / 8][(position.x) / 8 + w] != 2)
			return true;
	}
	return false;
}

bool ModulePlayer::CanClimbUp()
{
	if ((App->maps->map[(position.y + 31) / 8][(position.x + 12) / 8] == 2) || (App->maps->map[(position.y + 31) / 8][(position.x + 13) / 8] == 2))
		return true;
	else
		return false;
}

bool ModulePlayer::CanClimbDown()
{
	if ((App->maps->map[(position.y + 32) / 8][(position.x + 12) / 8] == 2) || (App->maps->map[(position.y + 32) / 8][(position.x + 13) / 8] == 2))
		return true;
	else
		return false;
}

void ModulePlayer::AlignLadder(int direction)
{
	if (ladderAlign == false)
	{
		position.x = GetLadderCenter(direction) * (8) - 8;
		ladderAlign = true;
	}
}

int ModulePlayer::GetLadderCenter(int direction)
{
	//We ensure that in (position.x + 12 / 8) we have a 2
	int x = (position.x + 12) / 8;

	if (App->maps->map[(position.y + 32 - direction) / 8][x - 1] != 2) // Case 0 2 2
	{
		LOG("right\n");
		return (x + 1);
	}

	else if (App->maps->map[(position.y + 32 - direction) / 8][x + 1] != 2) // Case 2 2 0
	{
		LOG("left\n");
		return (x - 1);
	}
	else // Case 2 2 2
	{
		LOG("mid\n");
		return (x);
	}
}