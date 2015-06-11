#include "Globals.h"
#include "Application.h"
#include "ModulePlayer2.h"
#include "ModuleGun.h"
#include <math.h>



// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModulePlayer2::ModulePlayer2(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModulePlayer2::~ModulePlayer2()
{
}

bool ModulePlayer2::Init()
{
	shieldAnim = &shield;
	score = 0;
	digitNumber = 0;
	pausePlayer = false;
	graphics = NULL;

	dead = false;

	//////////////
	//Animations//
	//////////////

	// idle animation
	idle.frames.PushBack({ 10 + 197, 112, 32, 32 });

	// idle2 animation
	idle2.frames.PushBack({ 14 + 197, 179 + 1, 28, 32 });
	// forward animation
	forward.frames.PushBack({ 10 + 197, 2, 32, 32 });
	forward.frames.PushBack({ 44 + 197, 2, 32, 32 });
	forward.frames.PushBack({ 78 + 197, 2, 32, 32 });
	forward.frames.PushBack({ 112 + 197, 2, 32, 32 });
	forward.frames.PushBack({ 10 + 197, 2, 32, 32 });
	forward.frames.PushBack({ 44 + 197, 2, 32, 32 });
	forward.frames.PushBack({ 78 + 197, 2, 32, 32 });
	forward.frames.PushBack({ 112 + 197, 2, 32, 32 });
	forward.frames.PushBack({ 146 + 197, 2, 32, 32 });
	forward.speed = 0.32f;

	// backward animation
	backward.frames.PushBack({ 14 + 197, 146, 28, 32 });
	backward.frames.PushBack({ 48 + 197, 146, 28, 32 });
	backward.frames.PushBack({ 82 + 197, 146, 28, 32 });
	backward.frames.PushBack({ 116 + 197, 146, 28, 32 });
	backward.frames.PushBack({ 14 + 197, 146, 28, 32 });
	backward.frames.PushBack({ 48 + 197, 146, 28, 32 });
	backward.frames.PushBack({ 82 + 197, 146, 28, 32 });
	backward.frames.PushBack({ 116 + 197, 146, 28, 32 });
	backward.frames.PushBack({ 150 + 197, 146, 28, 32 });
	backward.speed = 0.32f;


	// shot animation
	shot.frames.PushBack({ 44 + 197, 112, 32, 32 });

	// shot2 animation
	shot2.frames.PushBack({ 44 + 197, 179 + 1, 32, 32 });

	// climb animation
	climb.frames.PushBack({ 80 + 197, 36, 30, 32 });
	climb.frames.PushBack({ 47 + 197, 77, 30, 32 });
	climb.frames.PushBack({ 114 + 197, 36, 30, 32 });
	climb.frames.PushBack({ 47 + 197, 77, 30, 32 });

	climb.speed = 0.16f;

	endclimb.frames.PushBack({ 113 + 197, 78, 32, 32 });
	endclimb.speed = 0.0f;

	// animation
	//killDead back
	killDead.frames.PushBack({ 78 + 197, 112, 47, 32 });
	//killDead front
	killDead2.frames.PushBack({ 129 + 197, 112, 47, 32 });

	shield.frames.PushBack({ 0, 0, 33, 43 });
	shield.frames.PushBack({ 35, 0, 34, 43 });
	shield.speed = 0.2f;

	return true;
}


bool ModulePlayer2::Start()
{
	//Animation speed
	forward.speed = 0.32f;
	backward.speed = 0.32f;
	climb.speed = 0.16f;
	endclimb.speed = 0.0f;
	shield.speed = 0.2f;

	movementDirection = 1;

	lastHitBall = -1;
	speed = 2;
	current_animation = &idle;
	LOG("--Starting player");
	bool ret = true;
	graphics = App->scenePlay->livesGraphics;
	shieldTexture = App->textures->Load("./Image_Sources/Shield.png");
	if (graphics == NULL)
	{
		LOG("------------------Could not load player graphics----------------------");
	}
	playerState = standing;
	ladderAlign = false;

	deadAnimEnd = false;
	boost = none;

	App->player->deadFlashAnim.ResetLoops();
	App->player->deadFlashAnim.Reset();
	return ret;
}


update_status ModulePlayer2::Update()
{

	Reset();

	if (dead && !timeOut)
	{
		if (!App->player->deadFlashAnim.Finished())
			App->render->Blit(App->player->deadFlash, 0, 0, &App->player->deadFlashAnim.GetCurrentFrame(), NULL);
	}

	if (shieldDelay > 0)
	{
		shieldDelay--;
	}

	if (timeOut && !App->player->IsEnabled())
	{
		App->render->Blit(App->player->timeOutTexture, SCREEN_WIDTH / 2 - App->player->timeOutRect.w / 2, SCREEN_HEIGHT / 2 - App->player->timeOutRect.h / 2, &App->player->timeOutRect);
	}

	if (waitingContinue)
	{
		App->player->player2DeadTimer--;
	}
	if (!App->player->IsEnabled())
	{
		App->player->PrintInterface();
		if (App->scenePlay->debugMode == true)
		{
			App->player->PrintDebugMode();
		}
	}

	if (!waitingContinue)
	{
		if (timeOut)
		{
			App->render->Blit(App->player->timeOutTexture, SCREEN_WIDTH / 2 - App->player->timeOutRect.w / 2, SCREEN_HEIGHT / 2 - App->player->timeOutRect.h / 2, &App->player->timeOutRect);
		}
		if (shieldOn && !timeOut)
		{
			App->render->Blit(shieldTexture, position.x - 6, position.y - 8, &shieldAnim->GetCurrentFrame());
		}

		if (current_animation != NULL)
		{
			if ((shieldDelay / 4) % 2 == 0)
				App->render->Blit(graphics, position.x - 2, position.y, &current_animation->GetCurrentFrame());
		}

		if (!timeOut)
		{
			//////////////////////

			if (current_animation == &shot || current_animation == &shot2)
			{
				shotDelay++;
			}

			if (shotDelay >= 3)
			{
				shotDelay = 0;
				pausePlayer = false;
			}

			if (!pausePlayer)
			{
				if (dead == false)
				{
					SecurityPosition();
					IsFalling();
					EndClimbUp();
					StartClimbDown();
					Climb();
					Movement();
					Shoot();
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
			}
		}
	}


	return UPDATE_CONTINUE;
}

update_status ModulePlayer2::PostUpdate()
{
	if (!pausePlayer && !waitingContinue)
	{
	CheckBallCollision();
	}

	return UPDATE_CONTINUE;
}
bool ModulePlayer2::CleanUp()
{
	LOG("--Cleanup Player");
	return true;
}


//////  Player Methods //////


void ModulePlayer2::IsFalling()
{
	if ((App->maps->map[position.y / 8 + 4][(position.x + 6) / 8] == 0 && App->maps->map[position.y / 8 + 4][(position.x + 17) / 8] == 0 || MiddleLadder()) && (playerState != climbing && playerState != climbingUp && playerState != climbingDown))
	{
		playerState = falling;
	}
}

bool ModulePlayer2::LadderFall()
{
	int counter = 0;
	for (int w = 0; w < 32; w++)
	{
		if (App->maps->map[(position.y + 2) / 8 + 4][(position.x + w) / 8] == 2 && App->maps->map[(position.y + 2) / 8 + 3][(position.x + w) / 8] != 2)
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

void ModulePlayer2::SecurityPosition()
{
	if (position.y > SCREEN_HEIGHT - 9 * TILE)
		position.y = SCREEN_HEIGHT - 9 * TILE;
}

bool ModulePlayer2::MiddleLadder()
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


void ModulePlayer2::Movement()
{
	if (playerState != climbing && playerState != climbingUp && playerState != climbingDown)
	{
		if (movementDirection == 1)
			current_animation = &idle;
		if (movementDirection == -1)
			current_animation = &idle2;

		//Move right
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_LEFT) != KEY_REPEAT)
			{
				current_animation = &forward;
				bool canMove = true;
				for (int i = 0; i < 4; i++)
				{
					if (App->maps->map[position.y / 8 + i][(position.x + 25) / 8] == 1)
						canMove = false;
				}
				if (canMove)
				{
					position.x += speed;
					movementDirection = 1;
				}

			}
		}

		//Move left
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) != KEY_REPEAT)
			{
				current_animation = &backward;
				bool canMove = true;
				for (int i = 0; i < 4; i++)
				{
					if (App->maps->map[position.y / 8 + i][(position.x - 1) / 8] == 1)
						canMove = false;
				}

				if (canMove)
				{
					position.x -= speed;
					movementDirection = -1;
				}
			}
		}
	}
}

void ModulePlayer2::Shoot()
{
	if (App->gun->shootAvailable2 == true)
	{
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			pausePlayer = true;
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
			App->gun->Shoot((position + offset), 2);

		}
	}
}

void ModulePlayer2::Climb()
{
	if (playerState != falling && playerState != climbingUp && playerState != climbingDown)
	{
		if (playerState == climbing && App->input->GetKey(SDL_SCANCODE_UP) != KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_DOWN) != KEY_REPEAT)
			climb.speed = 0.0f;
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			{
				climb.speed = 0.0f;
			}
			else
			{

				if (playerState == climbing && LadderUpEnds())
				{
					LOG("LadderUpEnds\n");
					playerState = climbingUp;
					ladderAlign = false;
				}

				else if (CanClimbUp())
				{
					AlignLadder(1);
					climb.speed = 0.16f;
					playerState = climbing;
					current_animation = &climb;
					position.y -= 2;
				}

			}
		}

		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
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

				else if (playerState == climbing && NewLadderDown())
				{
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

void ModulePlayer2::Fall()
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

bool ModulePlayer2::EndClimbUp()
{
	if (playerState == climbingUp)
	{
		current_animation = &endclimb;
		if (finishClimbCounter < 4)
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

bool ModulePlayer2::StartClimbDown()
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

void ModulePlayer2::Kill(int xBallPos)
{
	LOG("Player has died\n");
	App->balls->pauseBalls = true;
	dead = true;
	App->audio->PlayMusic("./Sounds/Death.wav", 1);
	App->player->player2DeadTimer = 601;
	if (xBallPos != -1)
	{
		App->player->current_animation->speed = 0;
		App->player->pausePlayer = true;

		if (xBallPos < position.x + 16)
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
	else
	{
		current_animation->speed = 0;
		pausePlayer = true;
		timeOut = true;
	}

}

void ModulePlayer2::CheckBallCollision()
{
	if (!App->balls->pauseBalls)
	{
		p2List_item<Ball*>* tmp = App->balls->ballsList.getFirst();
		while (tmp != NULL && !dead)
		{
			if (!tmp->data->dead)
			{
				if ((tmp->data->position.y + tmp->data->radius >= position.y + 5) &&
					(tmp->data->position.y - tmp->data->radius <= position.y + 27) &&
					((tmp->data->position.x + tmp->data->radius) > position.x + 4) &&
					(tmp->data->position.x - tmp->data->radius) < position.x + 20)
				{
					if (shieldOn == true && !undying)
					{
						shieldDelay = 120;
						shieldOn = false;
						tmp->data->shieldKill = true;
					}
					else if (!undying && shieldDelay == 0)
					{
						dead = true;
						Kill(tmp->data->position.x);

					}
				}
			}

			tmp = tmp->next;
		}
	}
}

void ModulePlayer2::Reset()
{
	if (timeOut)
	{
		timeOutDelay++;
	}
	if (waitingContinue && App->player->player2DeadTimer <= 0)
	{
		App->scenePlay->player2Enabled = false;
		Disable();
	}
	if (deadAnimEnd == true)
	{
		if (App->scenePlay->lives2 > 0)
		{
			dead = false;
			App->scenePlay->lives2 -= 1;
			App->scenePlay->Disable();
			App->scenePlay->Enable(App->scenePlay->currentLvl);
		}
		else
		{
			if (!App->player->IsEnabled())
			{
				if (App->player->player2DeadTimer <= 0)
				{
					waitingContinue = false;
					App->scenePlay->player2Enabled = false;
					App->player->timeOutDelay = 0;
					App->player->timeOut = false;
					App->scenePlay->Disable();
					Disable();
					App->scenePlay->player2Enabled = false;
					App->sceneIntro->Enable();
				}
				else
					waitingContinue = true;
			}
			else
			{
				waitingContinue = true;
				if (App->player->player2DeadTimer <= 0)
				{
 					App->scenePlay->Disable();
					App->scenePlay->Enable(App->scenePlay->currentLvl);
				}
			}
		}
	}
	else if (timeOutDelay == 180)
	{
		if (App->player->IsEnabled() && !App->player->waitingContinue)
			App->scenePlay->lives1--;
		timeOutDelay = 0;
		timeOut = false;
		if (App->scenePlay->lives2 == 0)
			timeOutContinue = true;
		deadAnimEnd = true;
	}
}

bool ModulePlayer2::LadderUpEnds()
{
	for (int w = 0; w < 3; w++)
	{
		if (App->maps->map[(position.y + 25) / 8][(position.x) / 8 + w] == 0)
			if (App->maps->map[(position.y + 15) / 8][(position.x) / 8 + w] == 0)
				return true;
	}

	return false;
}

bool ModulePlayer2::LadderDownEnds()
{
	for (int w = 0; w < 3; w++)
	{
		if (((App->maps->map[(position.y + 32) / 8][(position.x + 13) / 8] != 2) && (App->maps->map[(position.y + 16) / 8][(position.x + 13) / 8] != 2)) ||
			(App->maps->map[(position.y + 32) / 8][(position.x + 13) / 8] == 1))
			return true;
	}
	return false;
}

bool ModulePlayer2::CanClimbUp()
{
	if ((App->maps->map[(position.y + 31) / 8][(position.x + 12) / 8] == 2) || (App->maps->map[(position.y + 31) / 8][(position.x + 13) / 8] == 2))
		return true;
	if ((App->maps->map[(position.y + 15) / 8][(position.x + 12) / 8] == 2) || (App->maps->map[(position.y + 15) / 8][(position.x + 13) / 8] == 2))
		return true;
	return false;
}

bool ModulePlayer2::CanClimbDown()
{
	if ((App->maps->map[(position.y + 32) / 8][(position.x + 12) / 8] == 2) || (App->maps->map[(position.y + 16) / 8][(position.x + 12) / 8] == 2))
		return true;
	else
		return false;
}

bool ModulePlayer2::CanStartClimbingDown()
{
	if ((App->maps->map[(position.y + 32) / 8][(position.x + 12) / 8] == 2) || (App->maps->map[(position.y + 32) / 8][(position.x + 13) / 8] == 2))
		return true;
	else
		return false;
}

bool ModulePlayer2::AlignLadder(int direction)
{
	if (ladderAlign == false)
	{
		position.x = GetLadderCenter(direction) * (8) - 8;
		LOG("%i", position.x / 8);
		ladderAlign = true;
		return true;
	}
	return false;
}

int ModulePlayer2::GetLadderCenter(int direction)
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

bool ModulePlayer2::NewLadderDown()
{
	bool a2 = false;
	bool a0 = false;
	for (int i = 0; i < 3; i++)
		if (App->maps->map[position.y / 8 + 4][position.x / 8 + i] == 2)
		{
			a2 = true;
		}
		else if (App->maps->map[position.y / 8 + 4][position.x / 8 + i] == 0)
		{
			a0 = true;
		}
	if (a2 && a0)
		return true;
	return false;
}
void ModulePlayer2::UpdateBoosts()
{

	if (boost != prevBoost)
	{
		App->gun->maxShots2 = 1;

		switch (boost)
		{
		case stayingHook:
		{
			App->gun->type2 = staying;
			App->gun->maxShots2 = 1;
			break;
		}
		case doubleHook:
		{
			App->gun->type2 = normal;
			App->gun->maxShots2 = 2;
			break;
		}
		case SMG:
		{
			App->gun->type2 = SMG;
			App->gun->maxShots2 = 50;
			break;
		}
		}
	}
	prevBoost = boost;
}