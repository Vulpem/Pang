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
	climb.frames.PushBack({ 81, 78, 30, 32 });
	climb.frames.PushBack({ 114, 36, 30, 32 });
	climb.frames.PushBack({ 81, 78, 30, 32 });

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
	position.y = 10 * TILE;

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
		std::cout << "Changed undying mode" << std::endl;
		undying = !undying;
	}
	if (undying == true)
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
	if (App->maps->map[position.y / 8 + 4][(position.x + 6) / 8] == 0 && App->maps->map[position.y / 8 + 4][(position.x + 17) / 8] == 0 && playerState != climbing)
	{
		playerState = falling;
	}
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
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		
		if (playerState == climbing && LadderUpEnds())
		{
			std::cout << "LadderUpEnds" << std::endl;
			playerState = standing;
		}
		
		else if (CanClimbUp())
		{
			playerState = climbing;
			current_animation = &climb;
			position.y--;
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
		if (App->maps->map[position.y / 8 + 4][(position.x + 6) / 8] != 0 && App->maps->map[position.y / 8 + 4][(position.x + 17) / 8] != 0)
		{
			//In case its a ladder

			fallCounter = 0;
			playerState = standing;
			speed = 2;			
			


		}

	}
}

void ModulePlayer::Kill()
{
	std::cout << "Player has died" << std::endl;

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

	if (App->maps->map[(position.y + 31) / 8][(position.x + 12)/ 8] == 0)
		return true;
	return false;
	/*
	for (int w = 0; w < 3; w++)
	{
		if (App->maps->map[(position.y + 30) / 8 + w][position.x / 8] == 0)
			std::cout << "Ladder up ends" << std::endl;
			return true;
	}
	return false;
	*/
}

/*
bool ModulePlayer::LadderUpEnds()
{
	bool ret = false;
	int w1 = 0;
	int tile = 2;

	for (int w1 = 0; w1 < 3, tile == 2; w1--)
	{
		tile = App->maps->map[tile_y][tile_x + w1];
	}

	w1--;

	for (int w2 = 0; w2 < 3; w2++)
	{
		if (App->maps->map[tile_y + 1][tile_x - w1 + w2] == 0)
		{
			std::cout << "LadderDownEnds" << std::endl;
			return true;
		}

	}

	return false;
}
*/

bool ModulePlayer::CanClimbUp()
{
	if ((App->maps->map[(position.y + 31) / 8][(position.x + 12) / 8] == 2) || (App->maps->map[(position.y + 31) / 8][(position.x + 13) / 8] == 2))
		return true;
	else
		return false;
}