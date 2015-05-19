#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleGun.h"
#include <math.h>


// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	}

ModulePlayer::~ModulePlayer()
{}

bool ModulePlayer::Init()
{
	punt = 0;

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
	endclimb.speed = 0.0f;

	// animation
	//killDead back
	killDead.frames.PushBack({ 78, 112, 47, 32 });
	//killDead front
	killDead2.frames.PushBack({ 129, 112, 47, 32 });

	return true;
}


bool ModulePlayer::Start()
{
	puntRect = { 0, 0, 0, 8};
	textRect = { 0, 0, 0, 8};
	LOG("--Starting player");
	bool ret = true;
	graphics = App->backgroundPlay->livesGraphics;
	if (graphics == NULL)
	{
		LOG("------------------Could not load player graphics----------------------");
	}
	playerState = standing;
	position.x = TILE;
	position.y = 9 * TILE;
	ladderAlign = false;
	dead = false;
	deadAnimEnd = false;
	boost = none;

	return ret;
}


update_status ModulePlayer::Update()
{
	//Printing interface//
	App->fonts->PrintText("PLAYER-1", textSurf, textRect, 2 * TILE, 26 * TILE, 8);
	App->fonts->PrintText("PLAYER-2", textSurf, textRect, 35 * TILE, 26 * TILE, 8);
	App->fonts->PrintText("MT.FUJI", textSurf, textRect, 20 * TILE, 26 * TILE, 8);
	App->fonts->PrintNumbers(punt, puntMessage, puntRect, 15 * TILE, 27 * TILE);
	if (App->backgroundPlay->lives > 4)
	{
		App->fonts->PrintNumbers(App->backgroundPlay->lives, textSurf, textRect, 10 * TILE + 18, 29 * TILE + 1);
	}
	//////////////////////

		if (dead == false)
		{
			SecurityPosition();
			IsFalling();
			Shoot();
			EndClimbUp();
			StartClimbDown();
			Climb();
			Movement();
			Fall();
			UpdateBoosts();
		}
		else
		{
			position.x += deadAnimXSpeed;
			position.y += deadAnimYSpeed;
			deadAnimYSpeed += 0.2;
			deadCounter++;
			if (deadCounter >= 150)
			{
				deadAnimEnd = true;
				deadCounter = 0;
			}
		}

	if (current_animation != NULL)
	{
			App->renderer->Blit(graphics, position.x - 2, position.y, &current_animation->GetCurrentFrame());
	}

	CheckBallCollision();


	return UPDATE_CONTINUE;
}

bool ModulePlayer::CleanUp()
{
	LOG("--Cleanup Player");
	return true;
}


//////  Player Methods //////


void ModulePlayer::IsFalling()
{
	if ((App->maps->map[position.y / 8 + 4][(position.x + 6) / 8] == 0 && App->maps->map[position.y / 8 + 4][(position.x + 17) / 8] == 0 || MiddleLadder()) && (playerState != climbing && playerState != climbingUp && playerState != climbingDown))
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
			return true;
		}

	}
	return false;
}

void ModulePlayer::SecurityPosition()
{
	if (position.y > SCREEN_HEIGHT - 9 * TILE)
		position.y = SCREEN_HEIGHT - 9 * TILE;
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
	if (playerState != climbing && playerState != climbingUp && playerState != climbingDown)
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
	if (playerState != falling && playerState != climbingUp && playerState != climbingDown)
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
				playerState = climbingUp;
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
				playerState = standing;
				ladderAlign = false;
			}
			
			else if (playerState == climbing && CanClimbDown())
			{
					climb.speed = 0.16f;
					position.y += 2;
			}
			else if (playerState == standing && CanStartClimbingDown())
			{
				AlignLadder(-1);
				position.y += 6;
				playerState = climbingDown;
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
		}
	}
}

bool ModulePlayer::EndClimbUp()
{
	if (playerState == climbingUp)
	{
		current_animation = &endclimb;
		if (finishClimbCounter < 7)
		{
			finishClimbCounter++;
		}
		else
		{
			position.y -= 6;
			playerState = standing;
			current_animation = &idle;
			finishClimbCounter = 0;
		}
		return true;
	}
	return false;
}

bool ModulePlayer::StartClimbDown()
{
	if (playerState == climbingDown)
	{
		current_animation = &endclimb;
		if (finishClimbCounter < 7)
		{
			finishClimbCounter++;
		}
		else
		{
			position.y += 4;
			playerState = climbing;
			current_animation = &climb;
			finishClimbCounter = 0;
		}
		return true;
	}
	return false;
}

void ModulePlayer::Kill(int xBallPos)
{
	LOG("Player has died\n");
	App->balls->pauseBalls = true;
	current_animation = &idle;
	playerState = dead;
	App->audio->PlayMusic("./Sounds/Death.wav");


	if (xBallPos < position.x+16)
	{
			current_animation = &killDead2;
			deadAnimXSpeed = 1;
	}
	else
	{
		current_animation = &killDead;
		deadAnimXSpeed = -1;
	}	
	deadAnimYSpeed = -4;
}

void ModulePlayer::CheckBallCollision()
{
	if (!App->balls->pauseBalls)
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
					dead = true;
					Kill(tmp->data->position.x);
				}
			}
			tmp = tmp->next;
		}
	}
		if (deadAnimEnd == true)
		{
			if (App->backgroundPlay->lives > 0)
			{
				App->backgroundPlay->lives -= 1;
				App->player->Disable();
				App->player->Enable();
				App->maps->LoadMap(App->backgroundPlay->currentLvl);
				App->balls->pauseBalls = false;
			}
			else
			{
				App->backgroundPlay->lives = 3;
				App->backgroundIntro->Enable();
				App->backgroundPlay->Disable();
			}
		}
	}



bool ModulePlayer::LadderUpEnds()
{
	for (int w = 0; w < 3; w++)
	{
		if (App->maps->map[(position.y + 25) / 8][(position.x) / 8 + w] == 0)
			if (App->maps->map[(position.y + 15) / 8][(position.x) / 8 + w] == 0)
				return true;
	}

	return false;
}

bool ModulePlayer::LadderDownEnds()
{
	for (int w = 0; w < 3; w++)
	{
		if (((App->maps->map[(position.y + 32) / 8][(position.x + 13) / 8] != 2) && (App->maps->map[(position.y + 16) / 8][(position.x + 13) / 8] != 2)) ||
			(App->maps->map[(position.y + 32) / 8][(position.x + 13) / 8] == 1))
		return true;
	}
	return false;
}

bool ModulePlayer::CanClimbUp()
{
	if ((App->maps->map[(position.y + 31) / 8][(position.x + 12) / 8] == 2) || (App->maps->map[(position.y + 31) / 8][(position.x + 13) / 8] == 2))
		return true;
	if ((App->maps->map[(position.y + 15) / 8][(position.x + 12) / 8] == 2) || (App->maps->map[(position.y + 15) / 8][(position.x + 13) / 8] == 2))
		return true;
	return false;
}

bool ModulePlayer::CanClimbDown()
{
	if ((App->maps->map[(position.y + 32) / 8][(position.x + 12) / 8] == 2) || (App->maps->map[(position.y + 16) / 8][(position.x + 12) / 8] == 2)) 
		return true;
	else
		return false;
}

bool ModulePlayer::CanStartClimbingDown()
{
	if ((App->maps->map[(position.y + 32) / 8][(position.x + 12) / 8] == 2) || (App->maps->map[(position.y + 32) / 8][(position.x + 13) / 8] == 2))
		return true;
	else
		return false;
}

bool ModulePlayer::AlignLadder(int direction)
{
	if (ladderAlign == false)
	{
		position.x = GetLadderCenter(direction) * (8) - 8;
		LOG("%i", position.x/8);
		ladderAlign = true;
		return true;
	}
	return false;
}

int ModulePlayer::GetLadderCenter(int direction)
{
	//We search for a 2
	int x = (position.x + 12) / 8;
	int y = 0;
	int tile = 0;

	if (direction == -1)
	{
		y = (position.y + 32) / 8;
		int w = 0;
		for (; w < 2 && tile != 2; ++w)
		{
			tile = App->maps->map[y][x + w];

		}
		if (w > 1)
			x++;
	}


	if (direction == 1)
	{

		for (int w = 0; w < 2, tile != 2; w++)
		{
			for (y = (position.y / 8); y < position.y / 8 + 4 && tile != 2; y++)
			{
				tile = App->maps->map[y][x + w];
			}
			if (w >= 1)
				x++;
		}

	}
	
	
	int b = App->maps->map[y][x];

	if (App->maps->map[y][x - 1] != 2) // Case 0 2 2
	{
		return (x + 1);
	}

	else if (App->maps->map[y][x + 1] != 2) // Case 2 2 0
	{
		return (x - 1);
	}
	else // Case 2 2 2
	{
		return (x);
	}
}

void ModulePlayer::UpdateBoosts()
{

	if (boost != prevBoost)
	{
		App->gun->maxShots = 1;

		switch (boost)
		{
		case doubleHook:
		{
			App->gun->maxShots = 2;
		}
		}
	}
	prevBoost = boost;
}