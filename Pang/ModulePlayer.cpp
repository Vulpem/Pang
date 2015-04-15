#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Maps.h"
#include "ModuleGun.h"

#include <iostream>
// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModulePlayer::ModulePlayer(Application* app) : Module(app)
{
	graphics = NULL;

	position.x = TILE;
	position.y = SCREEN_HEIGHT - 28 * TILE;


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
	LOG("Loading player textures");
	bool ret = true;
	graphics = App->textures->Load("./Image_Sources/Player.png"); // arcade version
	return ret;
}

// Update: draw background
update_status ModulePlayer::Update()
{

	int speed = 2;
	//////////////////////
	//Character movement//
	//////////////////////


	/////////////////
	//Player states//
	/////////////////

	if (map1[position.y / 8 + 4][(position.x + 6) / 8] == 0 && map1[position.y / 8 + 4][(position.x + 17) / 8] == 0 && playerState != climbing)
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
			if (App->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT && map1[position.y / 8][(position.x + 25) / 8] != 1)
			{
				current_animation = &forward;
				position.x += speed;
				movementDirection = 1;
			}

		}

		//Move left
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT && map1[position.y / 8][(position.x - 1) / 8] != 1)
			{
				current_animation = &backward;	
				position.x -= speed;
				movementDirection = - 1;
			}

		}
	}

	//Shot
	if (App->gun->shootAvailable == true)
	{
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			p2Point<int> offset;
			if (movementDirection == 1)
			{
				current_animation = &shot;
				offset.x = 10;
				offset.y = 0;
			}

			else if (movementDirection == -1)
			{
				current_animation = &shot2;
				offset.x = 5;
				offset.y = 0;
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

		if (map1[position.y / 8 + 4][position.x / 8] == 1)
		{
			fallCounter = 0;
			playerState = standing;
			speed = 2;
		}
	}

	//Climb

	if (playerState != falling)
	{
		if (map1[(position.y + 15) / 8][(position.x + 11) / 8] == 2 && map1[(position.y + 15) / 8][(position.x + 12) / 8] == 2 ||
			map1[(position.y + 31) / 8][(position.x + 11) / 8] == 2 && map1[(position.y + 31) / 8][(position.x + 12) / 8] == 2)
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
							k = map1[y + i][x + j];
						}
					}
					i--;
					j--;

					//Correct the position of the character
					if (map1[y + i][x + j - 1] != 2)
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
				if ((map1[(position.y + 30) / 8][(position.x + 12) / 8] == 2) &&
					(map1[(position.y + 29) / 8][(position.x + 12) / 8] != 2))
				{
					current_animation = &endclimb;
					playerState = standing;
					ladderAlign = false;
				}
				position.y -= 2;

			}

		}


		if (map1[position.y / 8 + 4][(position.x + 11) / 8] == 2 && map1[position.y / 8 + 4][(position.x + 12) / 8] == 2 ||
			map1[position.y / 8 + 2][(position.x + 11) / 8] == 2 && map1[position.y / 8 + 2][(position.x + 12) / 8] == 2)
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
						k = map1[y][x + i];
					}

					i--;

					//Correct the position of the character
					if (map1[y + i][x + i - 1] != 2)
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
					if ((map1[(position.y + 34) / 8][(position.x + 11) / 8] == KEY_REPEAT) || (map1[(position.y / 8) + 2][(position.x + 11) / 8] == KEY_REPEAT))
					{
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
		//App->renderer->Blit(graphics, position.x-2, position.y, &r);
		return UPDATE_CONTINUE;
}