#ifndef _SCENE2_INCLUDE
#define _SCENE2_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Lemming.h"
#include "TexturedQuad.h"
#include "Door.h"
#include "common_utils.h"
#include "Text.h"

#define SCENE2_WIDTH 848
#define SCENE2_HEIGHT 160
#define SCENE2_MAX_TIME 180

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene2
{

public:
	Scene2();
	~Scene2();

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
	ShaderProgram simpleTexProgram, maskedTexProgram, buttonsTexProgram;
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
	int backgroundMusicID;
	CameraPos cameraPos;
	bool faster;
	bool backgroundMusicPlaying;
	bool lemmingSelected;
	std::string powerLemmingSelected;
	Text uiText;

	bool win, lose;
	bool surrender;
};


#endif

