#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleInput : public Module
{
public:
	
	ModuleInput(Application* app);
	~ModuleInput();

	bool Init();
	update_status PreUpdate();
	bool CleanUp();

	const Uint8 *keyboard;
};