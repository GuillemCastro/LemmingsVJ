#ifndef _SCENE3_INCLUDE
#define _SCENE3_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Lemming.h"
#include "TexturedQuad.h"
#include "Door.h"
#include "common_utils.h"
#include "Text.h"
#include <map>

#define SCENE3_WIDTH 750
#define SCENE3_HEIGHT 160
#define SCENE3_MAX_TIME 180

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene3
{

public:
	Scene3();
	~Scene3();

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
	ShaderProgram simpleTexProgram, maskedTexProgram, buttonsTexProgram, bridgeProgram;
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

	std::map<int, int> powerNumbers;

	bool win, lose;
	bool surrender;

	Texture plainTexture;
	TexturedQuad* endScreen;
};


#endif

