#pragma once
#include "Texture.h"
#include "ShaderProgram.h"
#include "TexturedQuad.h"

class MenuScene
{
public:
	MenuScene();
	~MenuScene();
	void init();
	void update(int deltaTime);
	void render();
	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

private:
	void initShaders();

private:
	Texture colorTexture;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	TexturedQuad* menuScreen;
	Texture level1Tex, level2Tex, level3Tex;
	TexturedQuad* levelButton;
	float currentTime;
	glm::mat4 projection;
};

