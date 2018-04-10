#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"


Scene::Scene()
{
	map = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
}


void Scene::init()
{
	glm::vec2 geom[2] = {glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)-30)};
	glm::vec2 texCoords[2] = {glm::vec2(120.f / 512.0, 0.f), glm::vec2((120.f + 320.f) / 512.0f, 160.f / 256.0f)};
	glm::vec2 geomButton[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), 30.f) };
	glm::vec2 texCoordsButton[2] = { glm::vec2(0.0f,  0.0f), glm::vec2(1.f, 1.f) };



	initShaders();

	map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);
	colorTexture.loadFromFile("images/fun1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	maskTexture.loadFromFile("images/fun1_mask.png", TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);

	buttonQuad = TexturedQuad::createTexturedQuad(geomButton, texCoordsButton, simpleTexProgram);
	buttonsTexture.loadFromFile("images/buttons.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buttonsTexture.setMinFilter(GL_NEAREST);
	buttonsTexture.setMagFilter(GL_NEAREST);


	glm::vec2 texCoordsBridge[2] = { glm::vec2(0.0f, 0.f), glm::vec2(1.0f, 1.0f) };

	bridges = MaskedTexturedQuad::createTexturedQuad(geom, texCoordsBridge, maskedTexProgram);
	bridgeColorTexture.loadFromFile("images/bridge.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bridgeColorTexture.setMinFilter(GL_NEAREST);
	bridgeColorTexture.setMagFilter(GL_NEAREST);
	bridgesTextureMask.loadFromFile("images/bridge_mask.png", TEXTURE_PIXEL_FORMAT_L);
	bridgesTextureMask.setMinFilter(GL_NEAREST);
	bridgesTextureMask.setMagFilter(GL_NEAREST);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	
	//lemming.init(glm::vec2(60, 30), simpleTexProgram);
	//lemming.setMapMask(&maskTexture);
	for (int i = 0; i < 10; ++i) {
		lemmings[i].init(glm::vec2(60 + 10, 30), simpleTexProgram);
		lemmings[i].setMapMask(&maskTexture);
		lemmings[i].setBridges(&bridgesTextureMask);
		lemmingAlive[i] = 0;
	}
	lemmingAlive[0] = 1;

}

unsigned int x = 0;




void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	int init = currentTime / 2000;
	lemmingAlive[init] = true;
	for (int i = 0; i < 10; ++i) {
		if (lemmingAlive[i]) lemmings[i].update(deltaTime);
	}
}

void Scene::render()
{
	glm::mat4 modelview;

	maskedTexProgram.use();
	maskedTexProgram.setUniformMatrix4f("projection", projection);
	maskedTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	maskedTexProgram.setUniformMatrix4f("modelview", modelview);
	map->render(maskedTexProgram, colorTexture, maskTexture);

	maskedTexProgram.use();
	maskedTexProgram.setUniformMatrix4f("projection", projection);
	maskedTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	maskedTexProgram.setUniformMatrix4f("modelview", modelview);
	bridges->render(maskedTexProgram, bridgeColorTexture, bridgesTextureMask);

	
	maskedTexProgram.use();
	maskedTexProgram.setUniformMatrix4f("projection", projection);
	maskedTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	maskedTexProgram.setUniformMatrix4f("modelview", modelview);
	bridges->render(maskedTexProgram, buttonsTexture, bridgesTextureMask);
	
	
	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	modelview = glm::translate(modelview, glm::vec3(0.f, float(CAMERA_HEIGHT) - 30.f, 0.f));
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	//buttonQuad->render(buttonsTexture);


	//lemming.render();
	for (int i = 0; i < 10; ++i) {
		if (!lemmings[i].isAlive()) {
			lemmingAlive[i] = 0;
		}
		if (lemmingAlive[i]) lemmings[i].render();
	}
}

void Scene::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	if (bLeftButton) {
		cout << "clicked at " << mouseX/3 << " " << mouseY/3 << endl;
		Lemming* selectedLemming;
		bool selected = false;
		for (int i = 0; i < 10; ++i) {
			if (lemmings[i].insideCollisionBox(mouseX/3, mouseY/3)) {
				selected = true;
				selectedLemming = &lemmings[i];
			}
		}
		if (selected) {
			selectedLemming->setPower(EXPLOADER);
		}
	}

	if (bRightButton) {
		cout << "clicked at " << mouseX / 3 << " " << mouseY / 3 << endl;
		Lemming* selectedLemming;
		bool selected = false;
		for (int i = 0; i < 10; ++i) {
			if (lemmings[i].insideCollisionBox(mouseX / 3, mouseY / 3)) {
				selected = true;
				selectedLemming = &lemmings[i];
			}
		}
		if (selected) {
			selectedLemming->setPower(BLOCKER);
		}
	}
}

void Scene::eraseMask(int mouseX, int mouseY)
{
	int posX, posY;
	
	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	posX = mouseX/3 + 120;
	posY = mouseY/3;

	for(int y=max(0, posY-3); y<=min(maskTexture.height()-1, posY+3); y++)
		for(int x=max(0, posX-3); x<=min(maskTexture.width()-1, posX+3); x++)
			maskTexture.setPixel(x, y, 0);
}

void Scene::applyMask(int mouseX, int mouseY)
{
	int posX, posY;
	
	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	posX = mouseX/3 + 120;
	posY = mouseY/3;

	for(int y=max(0, posY-3); y<=min(maskTexture.height()-1, posY+3); y++)
		for(int x=max(0, posX-3); x<=min(maskTexture.width()-1, posX+3); x++)
			maskTexture.setPixel(x, y, 255);
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleTexProgram.init();
	simpleTexProgram.addShader(vShader);
	simpleTexProgram.addShader(fShader);
	simpleTexProgram.link();
	if(!simpleTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleTexProgram.log() << endl << endl;
	}
	simpleTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

	vShader.initFromFile(VERTEX_SHADER, "shaders/maskedTexture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/maskedTexture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	maskedTexProgram.init();
	maskedTexProgram.addShader(vShader);
	maskedTexProgram.addShader(fShader);
	maskedTexProgram.link();
	if(!maskedTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << maskedTexProgram.log() << endl << endl;
	}
	maskedTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

bool Scene::isALemmingAt(int x, int y) {
	bool lemmingThere = false;
	for (int i = 0; i < 10; ++i) {
		if (lemmings[i].insideCollisionBox(x, y) && lemmingAlive[i]) {
			lemmingThere = true;
		}
	}
	return lemmingThere;
}

