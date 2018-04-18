#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Lemming.h"
#include "Door.h"
#include "TexturedQuad.h"
#include <string>
#include "common_utils.h"
#include "Text.h"

#define SCENE1_WIDTH 520
#define SCENE1_HEIGHT 160
#define SCENE1_MAX_TIME 180 //in seconds

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

	void stop();

private:
	void initShaders();
	void eraseMask(int mouseX, int mouseY);
	void applyMask(int mouseX, int mouseY);
	void powerSelect(int powerNumber);
	void updateCamera();
	void checkLemmingSelected();
	bool lemmingAtExit();

private:
	Texture colorTexture;

	Texture buttonsTexture;
	TexturedQuad* buttonQuad;

	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	Texture bridgeColorTexture;
	VariableTexture bridgesTextureMask;
	MaskedTexturedQuad* bridges;
	ShaderProgram simpleTexProgram, maskedTexProgram, buttonsTexProgram, doorTexProgram;
	float currentTime;
	glm::mat4 projection;
	Door entryDoor;
	Door exitDoor;
	//Lemming lemming;
	Lemming lemmings[10];
	bool lemmingInit[10];
	int numLemmingsAlive, numLemmingsInit;
	LemmingPower powerSelected;
	int mouseX, mouseY;
	CameraPos cameraPos;
	bool faster;
	int backgroundMusicID;
	bool backgroundMusicPlaying;
	Texture entryDoorsheet;
	bool lemmingSelected;
	std::string powerLemmingSelected;
	Text uiText;

	Texture plainTexture;
	TexturedQuad* endScreen;

	bool win, lose;
	bool surrender;
};


#endif // _SCENE_INCLUDE

