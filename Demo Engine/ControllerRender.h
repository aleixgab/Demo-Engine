#ifndef __CONTROLLER_RENDER_H__
#define __CONTROLLER_RENDER_H__

#include "Controller.h"
#include "Shader.h"
#include <list>

class TextureImporter;
class GroundImporter;
class GameObject;

class ControllerRender : public Controller
{
public:

	ControllerRender(GameManager* mng);
	~ControllerRender();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	std::list<TextureImporter*> textures;
private:
	Shader basicShader;
	Shader particleShader;

	GroundImporter* ground;
};

#endif