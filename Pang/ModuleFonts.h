#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleFonts : public Module
{
public:
	ModuleFonts(Application* app, bool start_enabled = true);
	~ModuleFonts();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
public:
};