
#include "Globals.h"
#include "Application.h"
#include "ModuleMaps.h"

ModuleMaps::ModuleMaps(Application* app) : Module(app)
{

}

ModuleMaps::~ModuleMaps()
{

}

bool ModuleMaps::Start()
{

	return true;
}

update_status ModuleMaps::Update()
{

	return UPDATE_CONTINUE;
}
