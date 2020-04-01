#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

class GameManager;
struct PhysBody3D;

class Controller
{
public:
	GameManager* Mng;

	Controller(GameManager* parent) : Mng(parent)
	{}

	virtual ~Controller()
	{}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}
};
#endif