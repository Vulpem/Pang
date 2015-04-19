#include <math.h>
#include "Globals.h"
#include "Application.h"
#include "ModuleFadeToBlack.h"

ModuleFadeToBlack::ModuleFadeToBlack(Application* app, bool start_enabled) : Module(app, start_enabled),
start_time(0),
total_time(0),
fading_in(true),
mod_on(NULL),
mod_off(NULL)
{
	screen = { 0, 0, SCREEN_WIDTH * SCREEN_SIZE, SCREEN_HEIGHT * SCREEN_SIZE };
}

ModuleFadeToBlack::~ModuleFadeToBlack()
{}

// Update: draw background
update_status ModuleFadeToBlack::Update()
{
	if (start_time > 0)
	{
		Uint32 now = SDL_GetTicks() - start_time;
		float normalized = (float)now / (float)total_time;

		if (normalized > 1.0f)
			normalized = 1.0f;

		if (fading_in == false)
			normalized = 1.0f - normalized;

		App->renderer->DrawQuad(screen, 0, 0, 0, (Uint8)(normalized * 255.0f), false);

		if (now >= total_time)
		{
			if (fading_in == true)
			{
				total_time += total_time;
				fading_in = false;
				mod_off->Disable();
				mod_on->Enable();
				start_time = SDL_GetTicks();
			}
			else
				start_time = 0;
		}
	}

	return UPDATE_CONTINUE;
}

// Fade to black. At mid point deactivate one module, then activate the other
void ModuleFadeToBlack::FadeToBlack(Module* module_off, Module* module_on, float time)
{
	fading_in = true;
	start_time = SDL_GetTicks();
	total_time = (Uint32)(time * 0.5f * 1000.0f);
	mod_on = module_on;
	mod_off = module_off;
}