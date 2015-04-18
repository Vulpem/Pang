
#include "Globals.h"
#include "Application.h"
#include "ModuleMaps.h"


#include <iostream>

ModuleMaps::ModuleMaps(Application* app) : Module(app)
{

}

ModuleMaps::~ModuleMaps()
{

}

bool ModuleMaps::Start()
{


	for (int i = 0; i < 26; i++)
	{
		for (int j = 0; j < 48; j++)
		{
			std::cout << map[i][j];
		}
		std::cout<<std::endl;
	}

	return true;
}

update_status ModuleMaps::Update()
{

	return UPDATE_CONTINUE;
}
