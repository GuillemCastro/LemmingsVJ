#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "SoundManager.h"


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
	faster = false;
	win = false;
	lose = false;
	surrender = false;

	numLemmingsInit = 0;
	numLemmingsAlive = 10;

	backgroundMusicID = SoundManager::instance().play(LETSGO, false);
	backgroundMusicPlaying = false;

	glm::vec2 geom[2] = {glm::vec2(0.f, 0.f), glm::vec2(512.f/*float(CAMERA_WIDTH)*/, 256.f/*float(CAMERA_HEIGHT)-30*/)};
	glm::vec2 texCoords[2] = {glm::vec2(/*120.f / 512.0*/0.f, 0.f), glm::vec2(/*(120.f + 320.f) / 512.0f*/1.f, /*160.f / 256.0f*/1.f)};
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

	glm::vec2 texCoordsBridge[2] = { glm::vec2(0.0f, 0.f), glm::vec2(1.f, 1.f) };
	glm::vec2 geomBridge[2] = { glm::vec2(0.0f, 0.f), glm::vec2(512.f, 256.f) };

	bridges = MaskedTexturedQuad::createTexturedQuad(geomBridge, texCoordsBridge, maskedTexProgram);
	bridgeColorTexture.loadFromFile("images/bridge_fun1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bridgeColorTexture.setMinFilter(GL_NEAREST);
	bridgeColorTexture.setMagFilter(GL_NEAREST);
	bridgesTextureMask.loadFromFile("images/bridge_mask_fun1.png", TEXTURE_PIXEL_FORMAT_L);
	bridgesTextureMask.setMinFilter(GL_NEAREST);
	bridgesTextureMask.setMagFilter(GL_NEAREST);

	buttonQuad = TexturedQuad::createTexturedQuad(geomButton, texCoordsButton, buttonsTexProgram);
	buttonsTexture.loadFromFile("images/buttons.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buttonsTexture.setMinFilter(GL_NEAREST);
	buttonsTexture.setMagFilter(GL_NEAREST);

	glm::vec2 endScreenGeom[2] = {glm::vec2(0.f, 0.f), glm::vec2(CAMERA_WIDTH, CAMERA_HEIGHT)};
	glm::vec2 endScreenTexGeom[2] = {glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f)};
	endScreen = TexturedQuad::createTexturedQuad(endScreenGeom, endScreenTexGeom, simpleTexProgram);
	plainTexture.createEmptyTexture(CAMERA_WIDTH, CAMERA_HEIGHT);
	plainTexture.setMinFilter(GL_NEAREST);
	plainTexture.setMagFilter(GL_NEAREST);

	if (!uiText.init("fonts/OpenSans-Regular.ttf"))
		std::cout << "Could not load font!!!" << endl;

	cameraPos = {0, CAMERA_HEIGHT, 0, CAMERA_WIDTH};


	projection = glm::ortho(0.f, float(CAMERA_WIDTH), float(CAMERA_HEIGHT), 0.f);
	currentTime = 0.0f;

	entryDoor.init(glm::vec2(120 + 60, 10 + 30), doorTexProgram, true);
	exitDoor.init(glm::vec2(280, 55), doorTexProgram, false);
	
	for (int i = 0; i < 10; ++i) {
		lemmings[i].init(glm::vec2(120 + 60 + 10, 10 + 30), simpleTexProgram);
		lemmings[i].setMapMask(&maskTexture);
		lemmings[i].setBridges(&bridgesTextureMask);
		lemmingInit[i] = 0;
	}
	lemmingInit[0] = 1;

	powerNumbers[CLIMBER] = 0;
	powerNumbers[UMBRELLA] = 0;
	powerNumbers[EXPLOADER] = 1;
	powerNumbers[BLOCKER] = 0;
	powerNumbers[BUILDER] = 0;
	powerNumbers[BASHER] = 0;
	powerNumbers[MINER] = 0;
	powerNumbers[DIGGER] = 1;
}

void Scene::stop() {
	SoundManager::instance().stop(MUSIC1, backgroundMusicID);
}

//unsigned int x = 0;

void Scene::update(int deltaTime)
{
	if (!backgroundMusicPlaying && SoundManager::instance().isPlaying(LETSGO, backgroundMusicID)) {
		return;
	}
	else if (!backgroundMusicPlaying && !SoundManager::instance().isPlaying(MUSIC1, backgroundMusicID)) {
		backgroundMusicID = SoundManager::instance().play(MUSIC1, true);
		backgroundMusicPlaying = true;
	}
	if (faster)
		deltaTime *= 2;
	updateCamera();

	if (!win && !lose) {
		currentTime += deltaTime;
		int init = currentTime / 2000;

		if (!surrender)
			lemmingInit[init] = true;

		entryDoor.update(deltaTime);
		exitDoor.update(deltaTime);
		numLemmingsAlive = 0;
		numLemmingsInit = 0;
		for (int i = 0; i < 10; ++i) {
			if (lemmingInit[i]) {
				lemmings[i].update(deltaTime);
				++numLemmingsInit;
			}
			if (lemmings[i].isAlive()) {
				numLemmingsAlive++;
				if (surrender && lemmings[i].getPower() != EXPLOADER) {
					lemmings[i].setPower(EXPLOADER);
				}
			}
			
		}
		if (surrender && (10 - numLemmingsAlive) == numLemmingsInit) {
			lose = true;
		}
	}

	if (numLemmingsAlive > 10)
		numLemmingsAlive = 10;
	else if (numLemmingsAlive < 0)
		numLemmingsAlive = 0;
	if (numLemmingsInit > 10)
		numLemmingsInit = 10;
	else if (numLemmingsInit < 0)
		numLemmingsInit = 0;

	checkLemmingSelected();
	lose = lose || lemmingAtExit() || ((SCENE1_MAX_TIME - (currentTime / 1000)) <= 0);
	win = (win || (numLemmingsAlive == 0)) && !lose && !surrender;
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


	doorTexProgram.use();
	doorTexProgram.setUniformMatrix4f("projection", projection);
	doorTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	doorTexProgram.setUniformMatrix4f("modelview", modelview);
	entryDoor.render();
	exitDoor.render();

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

	buttonsTexProgram.use();
	buttonsTexProgram.setUniformMatrix4f("projection", projection);
	buttonsTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	modelview = glm::translate(modelview, glm::vec3(0.f + cameraPos.left, float(CAMERA_HEIGHT) - 30.f + cameraPos.top, 0.f));
	buttonsTexProgram.setUniformMatrix4f("modelview", modelview);
	buttonQuad->render(buttonsTexture);

	if (lemmingSelected) {
		uiText.render(this->powerLemmingSelected, glm::vec2(10, VIEWPORT_HEIGHT - 110), 40, glm::vec4(1.f, 1.f, 1.f, 1.f));
	}

	int secondsLeft = SCENE1_MAX_TIME - (currentTime/1000);
	if (secondsLeft < 0)
		secondsLeft = 0;
	char buff[100];
	sprintf(buff, "%d:%d%d", secondsLeft/60, (secondsLeft%60)/10, (secondsLeft%60)%10);
	uiText.render(buff, glm::vec2(VIEWPORT_WIDTH - 100, VIEWPORT_HEIGHT - 110), 40, glm::vec4(1.f, 1.f, 1.f, 1.f));

	sprintf(buff, "In: %d", numLemmingsInit);
	uiText.render(buff, glm::vec2(250, VIEWPORT_HEIGHT - 110), 40, glm::vec4(1.f, 1.f, 1.f, 1.f));

	sprintf(buff, "Dead: %d%%", (10-numLemmingsAlive)*10);
	uiText.render(buff, glm::vec2(400, VIEWPORT_HEIGHT - 110), 40, glm::vec4(1.f, 1.f, 1.f, 1.f));

	char buff2[50];
	int st = 185;
	int dist = 80;

	for (std::map<int, int>::iterator it = powerNumbers.begin(); it != powerNumbers.end(); ++it) {
		sprintf(buff2, "%d", it->second);
		uiText.render(buff2, glm::vec2(st, VIEWPORT_HEIGHT - 55), 40, glm::vec4(0.f, 0.f, 0.f, 1.f));
		st += dist;
	}

	if (win || lose) {
		simpleTexProgram.use();
		simpleTexProgram.setUniformMatrix4f("projection", projection);
		simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		modelview = glm::mat4(1.0f);
		modelview = glm::translate(modelview, glm::vec3(cameraPos.left, cameraPos.top, 0.f));
		simpleTexProgram.setUniformMatrix4f("modelview", modelview);

		endScreen->render(plainTexture);

		uiText.render("All lemmings accounted for.", glm::vec2(160.f, 140.f), 50, glm::vec4(0.f, 1.f, 0.f, 1.f));

		char buff2[100];
		sprintf(buff2, "You killed: %d%%", (10 - numLemmingsAlive) * 10);
		uiText.render(buff2, glm::vec2(300.f, 300.f), 50, glm::vec4(0.f, 1.f, 0.f, 1.f));

		sprintf(buff2, "You needed: %d%%", 100);
		uiText.render(buff2, glm::vec2(250.f, 350.f), 50, glm::vec4(0.f, 1.f, 0.f, 1.f));
		if (win) {
			uiText.render("YOU WIN!", glm::vec2(320.f, 500.f), 50, glm::vec4(0.f, 1.f, 0.4f, 1.f));
		}
		if (lose) {
			uiText.render("YOU LOSE!", glm::vec2(300.f, 500.f), 60, glm::vec4(1.f, 0.f, 0.2f, 1.f));
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

void Scene::updateCamera() {
	bool edited = false;
	if (cameraPos.bottom - cameraPos.top != CAMERA_HEIGHT) {
		cameraPos.top = 0;
		cameraPos.bottom = CAMERA_HEIGHT;
	}
	if (cameraPos.left - cameraPos.right != CAMERA_WIDTH) {
		if (cameraPos.left >= 0 && cameraPos.left <= SCENE1_WIDTH - CAMERA_WIDTH) {
			cameraPos.right = cameraPos.left + CAMERA_WIDTH;
		}
		else if (cameraPos.left >= CAMERA_WIDTH && cameraPos.left <= SCENE1_WIDTH) {
			cameraPos.left = cameraPos.right - CAMERA_WIDTH;
		}
		else {
			cameraPos.left = 0;
			cameraPos.right = CAMERA_WIDTH;
		}
	}
	CameraPos newPos = cameraPos;
	if (mouseX >= CAMERA_WIDTH - 30 && mouseX <= CAMERA_WIDTH && mouseY <= CAMERA_HEIGHT - 30) {
		if (cameraPos.right < (SCENE1_WIDTH) && cameraPos.left < cameraPos.right) {
			newPos.left = cameraPos.left + 1;
			newPos.right = cameraPos.right + 1;
			edited = true;
			//cout << "updating camera left right " << cameraPos.left << " " << cameraPos.right << endl;
		}
	}
	if (mouseX <= 30 && mouseX >= 0 && mouseY <= CAMERA_HEIGHT - 30) {
		if (cameraPos.left > 0 && cameraPos.left < cameraPos.right) {
			newPos.left = cameraPos.left - 1;
			newPos.right = cameraPos.right - 1;
			edited = true;
			//cout << "updating camera left right " << cameraPos.left << " " << cameraPos.right << endl;
		}
	}
	if (mouseY >= CAMERA_HEIGHT - 60 && mouseY <= CAMERA_HEIGHT - 30) {
		if (cameraPos.bottom < SCENE1_HEIGHT && cameraPos.top < cameraPos.bottom) {
			newPos.bottom = cameraPos.bottom + 1;
			newPos.top = cameraPos.top + 1;
			edited = true;
			//cout << "updating camera top bottom " << cameraPos.top << " " << cameraPos.bottom << endl;
		}
	}
	if (mouseY <= 30 && mouseY >= 0) {
		if (cameraPos.top >= 1 && cameraPos.top < cameraPos.bottom) {
			newPos.bottom = cameraPos.bottom - 1;
			newPos.top = cameraPos.top - 1;
			edited = true;
			//cout << "updating camera top bottom " << cameraPos.top << " " << cameraPos.bottom << endl;
		}
	}
	if (cameraPos.left >= cameraPos.right || cameraPos.top >= cameraPos.bottom) {
		cameraPos = {
			0, CAMERA_HEIGHT, 0, CAMERA_WIDTH
		};

	}
	if (edited && newPos.top < newPos.bottom && newPos.left < newPos.right
		&& newPos.bottom - newPos.top == CAMERA_HEIGHT && newPos.right - newPos.left == CAMERA_WIDTH) {
		cameraPos = newPos;
		cout << "update camera top: " << cameraPos.top << " bottom: " << cameraPos.bottom << " left: " << cameraPos.left << " right: " << cameraPos.right << endl;
		projection = glm::ortho(float(cameraPos.left), float(cameraPos.right), float(cameraPos.bottom), float(cameraPos.top));
	}
	else if (edited) {
		cout << "failt to update camera top: " << newPos.top << " bottom: " << newPos.bottom << " left: " << newPos.left << " right: " << newPos.right << endl;
		projection = glm::ortho(float(cameraPos.left), float(cameraPos.right), float(cameraPos.bottom), float(cameraPos.top));
	}
}

void Scene::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	this->mouseX = mouseX / 3;
	this->mouseY = mouseY / 3;

	int x = this->mouseX + (cameraPos.left);
	int y = this->mouseY + (cameraPos.top);

	if (bLeftButton) {
		cout << "clicked at " << mouseX/3 << " " << mouseY/3 << endl;

		if (this->mouseX >= 292 && this->mouseX <= 320 && this->mouseY >= CAMERA_HEIGHT - 30 && this->mouseY <= CAMERA_HEIGHT) {
			for (int i = 0; i < 10; ++i) {
				lemmings[i].setPower(EXPLOADER);
			}
			surrender = true;
		}

		if (this->mouseX >= 266 && this->mouseX <= 292 && this->mouseY >= CAMERA_HEIGHT - 30 && this->mouseY <= CAMERA_HEIGHT) {
			this->faster = !faster;
		}

		if (this->mouseY > (CAMERA_HEIGHT-30) && this->mouseY <= CAMERA_HEIGHT) {

			for (float i = 0.f; i < CAMERA_WIDTH; i += (CAMERA_WIDTH)/12) {
				if (this->mouseX >= i && this->mouseX < (i + CAMERA_WIDTH / 12)) {
					int powerNumber = (i / CAMERA_WIDTH) * 12 + 1;
					powerSelect(powerNumber);
				}
			}
		}

		Lemming* selectedLemming;
		bool selected = false;
		for (int i = 0; i < 10; ++i) {
			if (lemmings[i].insideCollisionBox(x, y)) {
				selected = true;
				selectedLemming = &lemmings[i];
			}
		}
		if (selected) {
			if (!(selectedLemming->getPower() == powerSelected) & powerNumbers[powerSelected] > 0) { //to not assign a power a lemming already has
				selectedLemming->setPower(powerSelected);
				powerNumbers[powerSelected] -= 1;
			}
		}
	}

	else if (bRightButton) {
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
	posX = mouseX/3 /*+ 120*/;
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
	posX = mouseX/3 /*+ 120*/;
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

	doorTexProgram.init();
	doorTexProgram.addShader(vShader);
	doorTexProgram.addShader(fShader);
	doorTexProgram.link();

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
		if (lemmings[i].insideCollisionBox(x + cameraPos.left, y + cameraPos.top) && lemmingInit[i]) {
			lemmingThere = true;
		}
	}
	return lemmingThere;
}

void Scene::checkLemmingSelected() {
	Lemming* selectedLemming;
	bool selected = false;
	for (int i = 0; i < 10; ++i) {
		if (lemmings[i].insideCollisionBox(mouseX + cameraPos.left, mouseY + cameraPos.top) && lemmingInit[i]) {
			selected = true;
			selectedLemming = &lemmings[i];
		}
	}
	if (selected) {
		this->powerLemmingSelected = powerToString(selectedLemming->getPower());
		this->lemmingSelected = true;
	}
	else {
		this->lemmingSelected = false;
	}
}

bool Scene::lemmingAtExit() {
	bool lemmingAtExit = false;
	for (int i = 0; i < 10; ++i) {
		if (lemmings[i].insideCollisionBox(exitDoor.position().x + 25, exitDoor.position().y + 15) && lemmingInit[i]) {
			lemmingAtExit = true;
		}
	}
	return lemmingAtExit;
}