#include "ModuleFonts.h"
#include "Application.h"
#include "Globals.h"

ModuleFonts::ModuleFonts(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleFonts::~ModuleFonts()
{

}

bool ModuleFonts::Start()
{
	return true;
}

update_status ModuleFonts::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleFonts::CleanUp()
{
	return true;
}