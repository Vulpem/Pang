
#pragma once

#include "Module.h"
#include "Globals.h"

class ModuleMaps : public Module
{
public:
	ModuleMaps(Application* app);
	~ModuleMaps();

	bool Start();
	update_status Update();

	int map[26][48];

};
