﻿#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>


void Game::init()
{
	state = MENU;
	bPlay = true;
	paused = false;
	faster = false;
	bLeftMouse = bRightMouse = false;

	cout << "loading sounds " << SoundManager::instance().init() << endl;
	//playSound(MUSIC1, true);

	glClearColor(0.f, 0.f, 0.f, 1.f/*0.3f, 0.3f, 0.3f, 1.0f*/);

	menuScene = new MenuScene();
	menuScene->init();

	initShaders();

	glm::vec2 geom[2] = {glm::vec2(0.f, 0.f), glm::vec2(150.0f, 75.0f)};
	glm::vec2 texCoords[2] = { glm::vec2(0.0f, 0.f), glm::vec2(1.0f, 1.0f) };

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);

	plainTexture.createEmptyTexture(200, 200);
	pausedQuad = TexturedQuad::createTexturedQuad(geom, texCoords, simpleTexProgram);

	if (!pausedText.init("fonts/OpenSans-Regular.ttf"))
		std::cout << "Could not load font!!!" << endl;

	glm::vec2 geomCursor[2] = { glm::vec2(0.0f, 0.0f), glm::vec2(10.f, 10.f) };

	cursorNormal.loadFromFile("images/cursor.png", TEXTURE_PIXEL_FORMAT_RGBA);
	cursorSelected.loadFromFile("images/cursorsel.png", TEXTURE_PIXEL_FORMAT_RGBA);
	cursor = TexturedQuad::createTexturedQuad(geomCursor, texCoords, simpleTexProgram);
}

bool Game::update(int deltaTime)
{
	SoundManager::instance().update();
	if (!paused) {
		switch (state) {
			case MENU: {
				if (scene1 != NULL) {
					//delete scene1;
					scene1 = NULL;
				}

				menuScene->update(deltaTime);
				lemmingSelected = false;
				break;
			}
			case SCENE1: {
				if (menuScene != NULL) {
					delete menuScene;
					menuScene = NULL;
				}
				scene1->update(deltaTime);
				int ratio = VIEWPORT_HEIGHT / CAMERA_HEIGHT;
				lemmingSelected = scene1->isALemmingAt(mouseX / ratio, mouseY / ratio);
				break;
			}
			case SCENE2: {
				if (menuScene != NULL) {
					delete menuScene;
					menuScene = NULL;
				}
				scene2->update(deltaTime);
				int ratio = VIEWPORT_HEIGHT / CAMERA_HEIGHT;
				lemmingSelected = scene2->isALemmingAt(mouseX / ratio, mouseY / ratio);
				break;
			}
			case SCENE3: {
				if (menuScene != NULL) {
					delete menuScene;
					menuScene = NULL;
				}
				scene3->update(deltaTime);
				int ratio = VIEWPORT_HEIGHT / CAMERA_HEIGHT;
				lemmingSelected = scene3->isALemmingAt(mouseX / ratio, mouseY / ratio);
				break;
			}
			case CREDITS: {
				if (menuScene != NULL) {
					delete menuScene;
					menuScene = NULL;
				}
				creditsScene->update(deltaTime);
				lemmingSelected = false;
				break;
			}
		}
	}	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switch (state) {
		case MENU: {
			menuScene->render();
			break;
		}
		case SCENE1: {
			scene1->render();
			break;
		}
		case SCENE2: {
			scene2->render();
			break;
		}
		case SCENE3: {
			scene3->render();
			break;
		}
		case CREDITS: {
			creditsScene->render();
			break;
		}
	}
	if (paused) {

		glm::mat4 modelview;

		simpleTexProgram.use();
		simpleTexProgram.setUniformMatrix4f("projection", projection);
		simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		modelview = glm::mat4(1.0f);
		modelview = glm::translate(modelview, glm::vec3(75.0f, 47.5f, 0.0f));
		simpleTexProgram.setUniformMatrix4f("modelview", modelview);

		pausedQuad->render(plainTexture);
		pausedText.render("MENU", glm::vec2(350, 240), 72, glm::vec4(1, 0.2f, 0.2f, 1));
		pausedText.render("Press \"P\" to resume", glm::vec2(360, 275), 20, glm::vec4(1, 0.2f, 0.2f, 1));
		pausedText.render("Press \"M\" to return to the menu", glm::vec2(310, 300), 20, glm::vec4(1, 0.2f, 0.2f, 1));
		pausedText.render("Press \"Q\" to quit the game", glm::vec2(320, 325), 20, glm::vec4(1, 0.2f, 0.2f, 1));

	}

	glm::mat4 modelview;
	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	modelview = glm::translate(modelview, glm::vec3(mouseX/3 -5, mouseY/3-5, 0.f));
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	if (lemmingSelected) {
		cursor->render(cursorSelected);
	}
	else {
		cursor->render(cursorNormal);
	}
}

void Game::keyPressed(int key)
{
	if (key == 27) // Escape code
	{
		if (state != MENU)
			paused = !paused;
		//bPlay = false;
	}
		
	if (key == 'P' || key == 'p') {
		if (state != MENU)
			paused = !paused;
	}
	if ((key == 'M' || key == 'm') && paused)
		this->changeScene(MENU);
	if ((key == 'Q' || key == 'q') && paused)
		bPlay = false;
	if (key == 'W' || key == 'w')
		faster = !faster;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
	mouseX = x;
	mouseY = y;
	switch (state) {
		case MENU: {
			menuScene->mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
			break;
		}
		case SCENE1: {
			scene1->mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
			break;
		}
		case SCENE2: {
			scene2->mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
			break;
		}
		case SCENE3: {
			scene3->mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
			break;
		}
		case CREDITS: {
			creditsScene->mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
			break;
		}
	}
}

void Game::mousePress(int button)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		cout << "clicked at x: " << mouseX << " y: " << mouseY << endl;
		bLeftMouse = true;
		switch (state) {
			case MENU: {
				menuScene->mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
				break;
			}
			case SCENE1: {
				scene1->mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
				break;
			}
			case SCENE2: {
				scene2->mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
				break;
			}
			case SCENE3: {
				scene3->mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
				break;
			}
			case CREDITS: {
				creditsScene->mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
				break;
			}
		}
	}
	else if(button == GLUT_RIGHT_BUTTON)
	{
		bRightMouse = true;
		switch (state) {
			case MENU: {
				menuScene->mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
				break;
			}
			case SCENE1: {
				scene1->mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
				break;
			}
			case SCENE2: {
				scene2->mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
				break;
			}
			case SCENE3: {
				scene3->mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
				break;
			}
			case CREDITS: {
				creditsScene->mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
				break;
			}
		}
	}
}

void Game::mouseRelease(int button)
{
	if(button == GLUT_LEFT_BUTTON)
		bLeftMouse = false;
	else if(button == GLUT_RIGHT_BUTTON)
		bRightMouse = false;
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

void Game::changeScene(GameScene scene) {
	if (this->state == scene) {
		return;
	}
	switch (state) {
		case SCENE1:
			this->scene1->stop();
			break;
		case SCENE2:
			this->scene2->stop();
			break;
		case SCENE3:
			this->scene3->stop();
			break;
		case CREDITS:
			break;
	}
	state = scene;
	paused = false;
	switch (scene) {
		case MENU:
			menuScene = new MenuScene;
			menuScene->init();
			break;
		case SCENE1:
			this->scene1 = new Scene;
			this->scene1->init();
			break;
		case SCENE2:
			this->scene2 = new Scene2;
			this->scene2->init();
			break;
		case SCENE3:
			this->scene3 = new Scene3;
			this->scene3->init();
			break;
		case CREDITS:
			this->creditsScene = new CreditsScene;
			this->creditsScene->init();
			break;
	}
}

void Game::initShaders() {
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleTexProgram.init();
	simpleTexProgram.addShader(vShader);
	simpleTexProgram.addShader(fShader);
	simpleTexProgram.link();
	if (!simpleTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleTexProgram.log() << endl << endl;
	}
	simpleTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

	vShader.initFromFile(VERTEX_SHADER, "shaders/maskedTexture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/maskedTexture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	maskedTexProgram.init();
	maskedTexProgram.addShader(vShader);
	maskedTexProgram.addShader(fShader);
	maskedTexProgram.link();
	if (!maskedTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << maskedTexProgram.log() << endl << endl;
	}
	maskedTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

int Game::timePerFrame() {
	if (faster) {
		return 1000.0f / 30.f;
	}
	else {
		return 1000.f / 30.f;
	}
}

bool Game::playSound(Sound sound, bool loop) {
	return SoundManager::instance().play(sound, loop);
}




