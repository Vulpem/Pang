#pragma once
#include "Module.h"
#include "ModuleAnimation.h"
#include "Globals.h"
#include "p2Point.h"

enum Player2States
{
	standing2,
	climbing2,
	falling2,
	climbing2Up,
	climbing2Down,
	caput2
};

class ModulePlayer2 : public Module
{
public:
	ModulePlayer2(Application* app, bool start_enabled = true);
	~ModulePlayer2();

	bool Init();
	bool Start();
	update_status Update();
	bool CleanUp();
	void Kill(int);
	bool pausePlayer;

public:

	// Movement variables//
	///////////////////////

	int boost = none;
	int prevBoost = none;
	int playerState = standing2;
	int fallCounter = 0;
	bool ladderAlign = false;
	int movementDirection = 1;
	int finishClimbCounter = 0;
	int deadCounter = 0;

	int speed = 2;
	bool undying = true;
	bool dead = false;
	bool deadAnimEnd = false;
	float deadAnimYSpeed;
	int deadAnimXSpeed;

	///////////////////////


	int score;
	int digitNumber;

	float rest;
	float div;
	int index;


	//Printing variables//
	SDL_Texture* textText;

//	SDL_Texture** uiText;
//	SDL_Rect* rectText;
	SDL_Rect textRect;

	//////////////////////

	p2Point<int> position;
	SDL_Texture* graphics;

	Animation* current_animation = NULL;

	Animation idle;
	Animation idle2;
	Animation forward;
	Animation backward;
	Animation climb;
	Animation endclimb;
	Animation shot;
	Animation shot2;
	Animation killDead;
	Animation killDead2;

private:

	//Movement methods//
	////////////////////
	void IsFalling();
	bool MiddleLadder();
	bool LadderFall();
	void SecurityPosition();

	void Movement();
	void Shoot();
	void Fall();
	void Climb();
	bool EndClimbUp();
	bool StartClimbDown();
	////////////////////

	void UpdateBoosts();

	void CheckBallCollision();
	void Reset();

	//Ladder Positioning//
	//////////////////////

	bool LadderUpEnds();
	bool LadderDownEnds();
	bool CanClimbUp();
	bool CanClimbDown();
	bool CanStartClimbingDown();
	bool AlignLadder(int direction);
	int GetLadderCenter(int direction);
	bool NewLadderDown();

	//////////////////////
	//Interface

	void PrintInterface();
	void PrintDebugMode();


};