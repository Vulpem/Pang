#pragma once
#include "Module.h"
#include "ModuleAnimation.h"
#include "Globals.h"
#include "p2Point.h"

enum UI_Player_messages
{
	UI_Player_MTFUJI = 1,
	UI_Player_MTKEIRIN,
	UI_Player_EMERALD,
	UI_Player_ANKOR_WATT,
	UI_Player_AUSTRALIA,
	UI_Player_TAJ_MAHAL,
	UI_Player_LENINGRAD,
	UI_Player_PARIS,
	UI_Player_LONDON,
	UI_Player_BARCELONA,
	UI_Player_ATHENS,
	UI_Player_EGYPT,
	UI_Player_KENYA,
	UI_Player_NEW_YORK,
	UI_Player_MAYA,
	UI_Player_ANTARTICA,
	UI_Player_EASTER,
	UI_Player_ISLAND,
	UI_Player_TEMPLE,
	UI_Player_FPS = 20,
	//	UI_Player_FPS_NUM,
	UI_Player_Frames,
	//	UI_Player_Frames_NUM,
	UI_Player_Time,
	//	UI_Player_Time_NUM,
	UI_Player_Player1,
	UI_Player_Player2,
	UI_Player_DASH,
	UI_Player_STAGE,
	UI_Player_PUSHBUTTON,
	UI_Player_MAX
};

enum PlayerStates
{
	standing,
	climbing,
	falling,
	climbingUp,
	climbingDown,
	caput
};

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	~ModulePlayer();

	bool Init();
	bool Start();
	update_status Update();
	bool CleanUp();
	void Kill(int);
	bool pausePlayer;
	void PrintInterface();
	void PrintDebugMode();

public:

	// Movement variables//
	///////////////////////

	int boost = none;
	bool shieldOn = false;
	int shieldDelay = 0;
	int prevBoost = none;
	int playerState = standing;
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

	int shotDelay = 0;

	///////////////////////


	int score;
	int digitNumber;

	float rest;
	float div;
	int index;
	

	//Printing variables//
	SDL_Texture* textText;

	SDL_Texture** uiText;
	SDL_Rect* rectText;
	SDL_Rect textRect;

	//////////////////////

	p2Point<int> position;
	SDL_Texture* graphics;
	SDL_Texture* shieldTexture;

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

	Animation* shieldAnim = NULL;
	Animation shield;

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




};