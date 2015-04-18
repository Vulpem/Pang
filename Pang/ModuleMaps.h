#pragma once

#include "Module.h"
#include "Globals.h"

class ModuleMaps : public Module
{
public:
	ModuleMaps(Application* app, bool start_enabled = true);
	~ModuleMaps();

	bool Start();
	update_status Update();

	static int map[26][48];

};
