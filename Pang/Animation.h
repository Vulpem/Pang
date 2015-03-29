#pragma once
#include "p2DynArray.h"

class Animation
{
public:
	float speed;
	p2DynArray<SDL_Rect> frames;

private:
	float current_frame;

public:
	Animation() : frames(5), speed(1.0f), current_frame(0)
	{}

	SDL_Rect& GetCurrentFrame()
	{
		if (this != NULL)
		{

		current_frame += speed;
		if (current_frame >= frames.Count())
			current_frame = 0;

		return frames[(int)current_frame];
		}
	}
};