#pragma once
#include "Texture.h"
#include "ShaderProgram.h"
#include "TexturedQuad.h"

class CreditsScene
{
public:
	CreditsScene();
	~CreditsScene();
	void init();
	void update(int deltaTime);
	void render();
	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

private:
	void initShaders();

private:
	Texture colorTexture;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	TexturedQuad* creditsScreen;
	float currentTime;
	glm::mat4 projection;
};

