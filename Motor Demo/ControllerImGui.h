#ifndef __CONTROLLER_IMGUI_H__
#define __CONTROLLER_IMGUI_H__

#include "Controller.h"

class ControllerImGui : public Controller
{
public:

	ControllerImGui(GameManager* mng);
	~ControllerImGui();

	bool Start();
	bool Update(float dt);

	void Hierarchy();
	void ObjInspector();

	void PlayStop();

	void Draw();
	
	bool CleanUp();

public:

};

#endif