#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"


Scene::Scene()
{
	map = NULL;
	bridges = NULL;
	buttonQuad = NULL;
}

Scene::~Scene()
{
	map->free();
	bridges->free();
	buttonQuad->free();
	if(map != NULL)
		delete map;
	if (bridges != NULL)
		delete bridges;
	if (buttonQuad != NULL)
		delete buttonQuad;

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

	glm::vec2 texCoordsBridge[2] = { glm::vec2(0.0f, 0.f), glm::vec2(1.0f, 1.0f) };
	glm::vec2 geomBridge[2] = { glm::vec2(0.0f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT) - 30) };

	bridges = MaskedTexturedQuad::createTexturedQuad(geomBridge, texCoordsBridge, maskedTexProgram);
	bridgeColorTexture.loadFromFile("images/bridge.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bridgeColorTexture.setMinFilter(GL_NEAREST);
	bridgeColorTexture.setMagFilter(GL_NEAREST);
	bridgesTextureMask.loadFromFile("images/bridge_mask.png", TEXTURE_PIXEL_FORMAT_L);
	bridgesTextureMask.setMinFilter(GL_NEAREST);
	bridgesTextureMask.setMagFilter(GL_NEAREST);

	buttonQuad = TexturedQuad::createTexturedQuad(geomButton, texCoordsButton, buttonsTexProgram);
	buttonsTexture.loadFromFile("images/buttons.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buttonsTexture.setMinFilter(GL_NEAREST);
	buttonsTexture.setMagFilter(GL_NEAREST);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	
	for (int i = 0; i < 10; ++i) {
		lemmings[i].init(glm::vec2(60 + 10, 30), simpleTexProgram);
		lemmings[i].setMapMask(&maskTexture);
		lemmings[i].setBridges(&bridgesTextureMask);
		lemmingInit[i] = 0;
	}
	lemmingInit[0] = 1;

}

unsigned int x = 0;

void Scene::update(int deltaTime)
{
	numLemmingsAlive = 0;
	currentTime += deltaTime;
	int init = currentTime / 2000;
	lemmingInit[init] = true;
	for (int i = 0; i < 10; ++i) {
		if (lemmings[i].isAlive()) {
			numLemmingsAlive++;
		}
		if (lemmingInit[i]) {
			lemmings[i].update(deltaTime);
		}
	}
}

void Scene::render()
{
	glm::mat4 modelview;

	buttonsTexProgram.use();
	buttonsTexProgram.setUniformMatrix4f("projection", projection);
	buttonsTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	modelview = glm::translate(modelview, glm::vec3(0.f, float(CAMERA_HEIGHT) - 30.f, 0.f));
	buttonsTexProgram.setUniformMatrix4f("modelview", modelview);
	buttonQuad->render(buttonsTexture);

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
	
	for (int i = 0; i < 10; ++i) {
		if (lemmingInit[i]) {
			simpleTexProgram.use();
			simpleTexProgram.setUniformMatrix4f("projection", projection);
			simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			modelview = glm::mat4(1.0f);
			simpleTexProgram.setUniformMatrix4f("modelview", modelview);
			lemmings[i].render();
		}
	}

	
}

void Scene::powerSelect(int powerNumber) {
	switch (powerNumber) {
		case 2: {
			powerSelected = CLIMBER;
			break;
		}
		case 3: {
			powerSelected = UMBRELLA;
			break;
		}
		case 4: {
			powerSelected = EXPLOADER;
			break;
		}
		case 5: {
			powerSelected = BLOCKER;
			break;
		}
		case 6: {
			powerSelected = BUILDER;
			break;
		}
		case 7: {
			powerSelected = BASHER;
			break;
		}
		case 9: {
			powerSelected = DIGGER;
			break;
		}
		default: {
			powerSelected = NONE;
			break;
		}
	}
	return;
}

void Scene::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	int x = mouseX / 3;
	int y = mouseY / 3;
	if (bLeftButton) {
		cout << "clicked at " << mouseX/3 << " " << mouseY/3 << endl;

		if (x >= 292 && x <= 320 && y >= 160 && y <= 190) {
			for (int i = 0; i < 10; ++i) {
				if (lemmingInit[i]) {
					lemmings[i].setPower(EXPLOADER);
				}
			}
		}

		if (y > (CAMERA_HEIGHT-30) && y <= CAMERA_HEIGHT) {

			for (float i = 0.f; i < CAMERA_WIDTH; i += (CAMERA_WIDTH)/12) {
				if (x >= i && x < (i + CAMERA_WIDTH / 12)) {
					int powerNumber = (i / CAMERA_WIDTH) * 12 + 1;
					powerSelect(powerNumber);
				}
			}
		}

		Lemming* selectedLemming;
		bool selected = false;
		for (int i = 0; i < 10; ++i) {
			if (lemmings[i].insideCollisionBox(mouseX/3, mouseY/3)) {
				selected = true;
				selectedLemming = &lemmings[i];
			}
		}
		if (selected) {
			selectedLemming->setPower(powerSelected);
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

	buttonsTexProgram.init();
	buttonsTexProgram.addShader(vShader);
	buttonsTexProgram.addShader(fShader);
	buttonsTexProgram.link();

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
		if (lemmings[i].insideCollisionBox(x, y) && lemmingInit[i]) {
			lemmingThere = true;
		}
	}
	return lemmingThere;
}

