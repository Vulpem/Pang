#include "Globals.h"
#include "Application.h"
#include "ModuleFonts.h"

ModuleFonts::ModuleFonts(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleFonts::~ModuleFonts()
{}

bool ModuleFonts::Init()
{
	return true;
}

update_status ModuleFonts::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleFonts::Update()
{
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleFonts::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleFonts::CleanUp()
{
	return true;
}