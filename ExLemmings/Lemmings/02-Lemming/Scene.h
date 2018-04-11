#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Lemming.h"
#include "TexturedQuad.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	void spawnLemming(int i);
	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);
	bool isALemmingAt(int x, int y);

private:
	void initShaders();
	void eraseMask(int mouseX, int mouseY);
	void applyMask(int mouseX, int mouseY);
	void powerSelect(int powerNumber);

private:
	Texture colorTexture;

	Texture buttonsTexture;
	TexturedQuad* buttonQuad;

	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	Texture bridgeColorTexture;
	VariableTexture bridgesTextureMask;
	MaskedTexturedQuad* bridges;
	ShaderProgram simpleTexProgram, maskedTexProgram, buttonsTexProgram;
	float currentTime;
	glm::mat4 projection;
	//Lemming lemming;
	Lemming lemmings[10];
	bool lemmingInit[10];
	int numLemmingsAlive;
	LemmingPower powerSelected;
};


#endif // _SCENE_INCLUDE

