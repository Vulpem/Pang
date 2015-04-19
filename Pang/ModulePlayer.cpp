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
	backward.frames.PushBack({ 10, 146, 32, 32 });
	backward.frames.PushBack({ 44, 146, 32, 32 });
	backward.frames.PushBack({ 78, 146, 32, 32 });
	backward.frames.PushBack({ 112, 146, 32, 32 });
	backward.frames.PushBack({ 10, 146, 32, 32 });
	backward.frames.PushBack({ 44, 146, 32, 32 });
	backward.frames.PushBack({ 78, 146, 32, 32 });
	backward.frames.PushBack({ 112, 146, 32, 32 });
	backward.frames.PushBack({ 146, 146, 32, 32 });
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

// Load assets
bool ModulePlayer::Start()
{
	LOG("--Initializing player");
	bool ret = true;
	graphics = App->textures->Load("./Image_Sources/Player.png"); // arcade version

	position.x = TILE;
	position.y = SCREEN_HEIGHT - 28 * TILE;

	dead = false;

	return ret;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	LOG("--Updating Player")
	int speed = 2;
	//////////////////////
	//Character movement//
	//////////////////////


		/////////////////
		//Player states//
		/////////////////

	if (App->maps->map[position.y / 8 + 4][(position.x + 6) / 8] == 0 && App->maps->map[position.y / 8 + 4][(position.x + 17) / 8] == 0 && playerState != climbing)
	{
		playerState = falling;
	}

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
				movementDirection = - 1;
			}

		}
	}

	//Shoot

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
			App->gun->Shoot((position+offset));
	
		}
	}

	//Fall
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
		if (App->maps->map[position.y / 8 + 4][position.x / 8] == 1)
		{
			int b = App->maps->map[25][1];
			fallCounter = 0;
			playerState = standing;
			speed = 2;
		}
	}

	//Climb

	if (playerState != falling)
	{
		if (App->maps->map[(position.y + 15) / 8][(position.x + 11) / 8] == 2 && App->maps->map[(position.y + 15) / 8][(position.x + 12) / 8] == 2 ||
			App->maps->map[(position.y + 31) / 8][(position.x + 11) / 8] == 2 && App->maps->map[(position.y + 31) / 8][(position.x + 12) / 8] == 2)
		{
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			{
				//Align position with the ladder
				if (!ladderAlign)
				{
					int x = position.x / 8;
					int y = (position.y / 8) + 1;

					int i = 0;
					int j = 0;

					//Search for a 2
					for (int k = 0; k != 2; i++)
					{
						for (j = 0; k != 2; j++)
						{
							k = App->maps->map[y + i][x + j];
						}
					}
					i--;
					j--;

					//Correct the position of the character
					if (App->maps->map[y + i][x + j - 1] != 2)
					{
						position.x = ((position.x / 8) + j) * 8;
					}

					else
					{
						position.x = ((position.x / 8) + j - 1) * 8;
					}
					ladderAlign = true;
				}



				climb.speed = 0.16f;
				playerState = climbing;
				current_animation = &climb;

				//Check if the ladder ends
				if ((App->maps->map[(position.y + 30) / 8][(position.x + 12) / 8] == 2) &&
					(App->maps->map[(position.y + 29) / 8][(position.x + 12) / 8] == 0))
				{
					current_animation = &endclimb;
					playerState = standing;
					ladderAlign = false;
				}
				position.y -= 2;

			}

		}


		if (App->maps->map[position.y / 8 + 4][(position.x + 11) / 8] == 2 && App->maps->map[position.y / 8 + 4][(position.x + 12) / 8] == 2 ||
			App->maps->map[position.y / 8 + 2][(position.x + 11) / 8] == 2 && App->maps->map[position.y / 8 + 2][(position.x + 12) / 8] == 2)
		{
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			{
				//Align position with the ladder
				if (!ladderAlign)
				{
					int x = position.x / 8;
					int y = (position.y / 8) + 4;

					int i = 0;

					//Search for a 2
					for (int k = 0; k != 2; i++)
					{
						k = App->maps->map[y][x + i];
					}

					i--;

					//Correct the position of the character
					if (App->maps->map[y + i][x + i - 1] != 2)
					{
						position.x = ((position.x / 8) + i) * 8;
					}

					else
					{
						position.x = ((position.x / 8) + i - 1) * 8;
					}
					ladderAlign = true;
				}

					climb.speed = 0.16f;
					playerState = climbing;
					current_animation = &climb;

					//Check if the ladder ends
					if ((App->maps->map[((position.y + 2) / 8) + 2][(position.x + 11) / 8] != 2))
			
					{
						if ((App->maps->map[(position.y + 35) / 8][(position.x + 11) / 8] != 2))
						playerState = standing;
						ladderAlign = false;
					}
					position.y+=2;
				}
			}


			//Stop the climbing animation
		if (playerState == climbing && App->input->GetKey(SDL_SCANCODE_W) != KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_S) != KEY_REPEAT)
			{
				climb.speed = 0.0f;
			}
	}


		if (current_animation != NULL)
		{
			SDL_Rect r = current_animation->GetCurrentFrame();
			App->renderer->Blit(graphics, position.x - 2, position.y, &r);

		}

		///////////////////////////
		//Checking Ball collision//
		///////////////////////////		
		CheckBallCollision();


		return UPDATE_CONTINUE;
}

bool ModulePlayer::CleanUp()
{
	LOG("--Cleanup Player");

	App->textures->Unload(graphics);

	return true;
}

void ModulePlayer::CheckBallCollision()
{
		p2List_item<Ball*>* tmp = App->balls->ballsList.getFirst();
		while (tmp != NULL && !dead)
		{
			if ((tmp->data->position.y + tmp->data->radius >= position.y + 5 ) &&
				(tmp->data->position.y - tmp->data->radius <= position.y + 27) &&
				((tmp->data->position.x + tmp->data->radius) > position.x + 5) &&
				(tmp->data->position.x - tmp->data->radius) < position.x + 25)
			{
				Kill();
				App->balls->pauseBalls = true;
				dead = true;
			}
			tmp = tmp->next;
		}
}

void ModulePlayer::Kill()
{
		std::cout << "Player has died" << std::endl;
		if (undying == false)
		{
			App->fade->FadeToBlack(App->backgroundPlay, App->backgroundIntro, 3.0f);
		}
}