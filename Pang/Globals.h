#pragma once
#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);
 
int CountDigits(int number);

#define MIN(a, b) (a<b ? a:b)
#define MAX(a, b) (a>b ? a:b)

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

enum BallTypes
{
	little = 0,
	medium,
	big,
	huge
};

enum BallColours
{
	red = 0,
	blue,
	green
};

enum Boosts
{
	none = 0,
	stayingHook,
	doubleHook,
	gun,
	life,
	pause,
	bomb,
	shield,
};



// Configuration -----------
#define TILE 8

#define FPS 60
#define SCREEN_WIDTH 384
#define SCREEN_HEIGHT 240
#define SCREEN_SIZE 2
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE false
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "Pang"

# define PAUSE_BOOST_TIME 4.0f
