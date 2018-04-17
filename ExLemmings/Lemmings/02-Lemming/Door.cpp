#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Door.h"
#include "Game.h"
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <mmsystem.h>
#include <algorithm>


enum DoorAnims
{
	EntryDoor, ExitDoor
};

void Door::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, bool entryDoor)
{
	spritesheet.loadFromFile("images/doorSpritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);

	float numAnim = 2.0f;
	sprite = Sprite::createSprite(glm::ivec2(42, 24), glm::vec2(0.1, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(numAnim);

	sprite->setAnimationSpeed(EntryDoor, 12);
	float y = 0.0f;
	for (int i = 0; i<9; i++)
		sprite->addKeyframe(EntryDoor, glm::vec2(float(i) / 10, y));

	sprite->setAnimationSpeed(ExitDoor, 12);
	y = 0.5f;
	for (int i = 0; i<6; i++)
		sprite->addKeyframe(ExitDoor, glm::vec2(float(i) / 10, y));




	if (entryDoor) sprite->changeAnimation(EntryDoor);
	else sprite->changeAnimation(ExitDoor);
	sprite->setPosition(initialPosition);
}



void Door::render()
{
	sprite->render();
}

void Door::update(int deltaTime)
{
	if (sprite->getCurrentKeyframe() == 8) return;
	else if (sprite->update(deltaTime) == 0)
		return;
}

glm::vec2 Door::position() const {
	return sprite->position();
}
