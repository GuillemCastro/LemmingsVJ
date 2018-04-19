#include "MenuScene.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Game.h"

MenuScene::MenuScene()
{
}


MenuScene::~MenuScene()
{
	menuScreen->free();
	levelButton->free();
	delete menuScreen;
	delete levelButton;
}

void MenuScene::init() {
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT) - 60) };
	glm::vec2 geomButtons[2] = {glm::vec2(0.f, 0.f), glm::vec2(50.0f, 25.0f)};
	glm::vec2 texCoords[2] = { glm::vec2(0.0f, 0.f), glm::vec2(1.0f, 1.0f) };

	initShaders();

	menuScreen = TexturedQuad::createTexturedQuad(geom, texCoords, simpleTexProgram);
	levelButton = TexturedQuad::createTexturedQuad(geomButtons, texCoords, simpleTexProgram);
	colorTexture.loadFromFile("images/title.png", TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);

	level1Tex.loadFromFile("images/level1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	level1Tex.setMinFilter(GL_NEAREST);
	level1Tex.setMagFilter(GL_NEAREST);

	level2Tex.loadFromFile("images/level2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	level2Tex.setMinFilter(GL_NEAREST);
	level2Tex.setMagFilter(GL_NEAREST);

	level3Tex.loadFromFile("images/level3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	level3Tex.setMinFilter(GL_NEAREST);
	level3Tex.setMagFilter(GL_NEAREST);

	creditsTex.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	creditsTex.setMagFilter(GL_NEAREST);
	creditsTex.setMinFilter(GL_NEAREST);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void MenuScene::update(int deltaTime) {
	currentTime += deltaTime;
}

void MenuScene::render() {
	glm::mat4 modelview;
	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	modelview = glm::translate(modelview, glm::vec3(0.f, 30.f, 0.f));
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);

	menuScreen->render(colorTexture);

	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", .0f, 1.0f, .0f, 1.0f);
	modelview = glm::mat4(1.0f);
	modelview = glm::translate(modelview, glm::vec3(55.f, 130.0f, 0.0f));
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);

	levelButton->render(level1Tex);

	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", .0f, .0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	modelview = glm::translate(modelview, glm::vec3(135.f, 130.0f, 0.0f));
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);

	levelButton->render(level2Tex);

	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 0.f, .0f, 1.0f);
	modelview = glm::mat4(1.0f);
	modelview = glm::translate(modelview, glm::vec3(225.f, 130.0f, 0.0f));
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);

	levelButton->render(level3Tex);

	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", .5f, .5f, .5f, 1.0f);
	modelview = glm::mat4(1.0f);
	modelview = glm::translate(modelview, glm::vec3(135.f, 160.0f, 0.0f));
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);

	levelButton->render(creditsTex);


}

void MenuScene::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton) {

	int x = mouseX / 3;
	int y = mouseY / 3;

	if (bLeftButton) {

		if (x >= 55 && x <= 105 && y >= 130 && y <= 155) {
			Game::instance().changeScene(SCENE1);
		}
		else if (x >= 135 && x <= 185 && y >= 130 && y <= 155) {
			Game::instance().changeScene(SCENE2);
		}
		else if (x >= 225 && x <= 275 && y >= 130 && y <= 155) {
			Game::instance().changeScene(SCENE3);
		}

		else if (x >= 135 && x <= 185 && y >= 160 && y <= 185) {
			Game::instance().changeScene(CREDITS);
		}
	}

}

void MenuScene::initShaders() {
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
