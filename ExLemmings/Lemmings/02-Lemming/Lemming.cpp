#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Lemming.h"
#include "Game.h"
#include <algorithm>


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum LemmingAnims
{
	WALKING_LEFT, WALKING_RIGHT, FALLING_RIGHT
};

void Lemming::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	state = FALLING_RIGHT_STATE;
	spritesheet.loadFromFile("images/lemmingfalling.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125, 0.33), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);
	
		sprite->setAnimationSpeed(WALKING_RIGHT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(WALKING_RIGHT, glm::vec2(float(i) / 8, 0.0f));
		
		sprite->setAnimationSpeed(WALKING_LEFT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(WALKING_LEFT, glm::vec2(float(i) / 8, 0.34f));

		sprite->setAnimationSpeed(FALLING_RIGHT, 12);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(FALLING_RIGHT, glm::vec2(float(i) / 8, 0.67f));

	
	sprite->changeAnimation(WALKING_RIGHT);
	sprite->setPosition(initialPosition);
}


void Lemming::update(int deltaTime)
{
	int fall;

	if(sprite->update(deltaTime) == 0)
		return;

	switch(state)
	{
	case FALLING_LEFT_STATE:
		fall = collisionFloor(2);
		if(fall > 0)
			sprite->position() += glm::vec2(0, fall);
		else
			state = WALKING_LEFT_STATE;
		break;
	case FALLING_RIGHT_STATE:
		fall = collisionFloor(2);
		if(fall > 0)
			sprite->position() += glm::vec2(0, fall);
		else {
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
		}
		break;
	case WALKING_LEFT_STATE:
		sprite->position() += glm::vec2(-1, -1);
		if(collision())
		{
			sprite->position() -= glm::vec2(-1, -1);
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
		}
		else
		{
			fall = collisionFloor(3);
			if(fall > 0)
				sprite->position() += glm::vec2(0, 1);
			if(fall > 1)
				sprite->position() += glm::vec2(0, 1);
			if(fall > 2)
				//sprite->changeAnimation(FALLING_RIGHT);
				state = FALLING_LEFT_STATE;		
		}
		break;
	case WALKING_RIGHT_STATE:
		sprite->position() += glm::vec2(1, -1);
		if(collision())
		{
			sprite->position() -= glm::vec2(1, -1);
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
		}
		else
		{
			fall = collisionFloor(3);
			if(fall < 3)
				sprite->position() += glm::vec2(0, fall);
			else
				sprite->changeAnimation(FALLING_RIGHT);
				state = FALLING_RIGHT_STATE;
		}
		break;
	case DIGGER_STATE: {
		fall = collisionFloor(2);
		if (fall > 0) {
			state = FALLING_RIGHT_STATE;
			sprite->changeAnimation(FALLING_RIGHT);
		}
		else {
			sprite->position() += glm::vec2(0, 1);
			for (int x = max(0.0f, sprite->position().x + 4); x <= min(mask->width() - 1.0f, sprite->position().x + 8.0f); ++x) {
				mask->setPixel(x + 120, sprite->position().y + 15, 0);
			}
		}
		break;
	}
	}
}

void Lemming::render()
{
	sprite->render();
}

void Lemming::setMapMask(VariableTexture *mapMask)
{
	mask = mapMask;
}

bool Lemming::insideCollisionBox(int x, int y) {
	//cout << "collision box for this lemming is x_min: " << sprite->position().x << " x_max: " << sprite->position().x + 16 << endl;
	//cout << "    " << "y_min: " << sprite->position().y << " y_max: " << sprite->position().y + 16 << endl;
	if ((x >= sprite->position().x) && (x <= sprite->position().x + 16)
		&& (y >= sprite->position().y) && (y <= sprite->position().y + 16)) {
		return true;
	}
	return false;
}

int Lemming::collisionFloor(int maxFall)
{
	bool bContact = false;
	int fall = 0;
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 16);
	while((fall < maxFall) && !bContact)
	{
		if((mask->pixel(posBase.x, posBase.y+fall) == 0) && (mask->pixel(posBase.x+1, posBase.y+fall) == 0))
			fall += 1;
		else
			bContact = true;
	}

	return fall;
}

bool Lemming::collision()
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 15);
	if((mask->pixel(posBase.x, posBase.y) == 0) && (mask->pixel(posBase.x+1, posBase.y) == 0))
		return false;

	return true;
}

void Lemming::setPower(LemmingPower power) {

	bool left = false;
	if (state == WALKING_LEFT_STATE || state == FALLING_LEFT_STATE || state == BASHER_LEFT
		|| state == CLIMBER_LEFT || state == BUILDER_LEFT) {
		left = true;
	}

	switch (power) {
	case DIGGER: {
		state = DIGGER_STATE;
		break;
	}

	case BASHER: {
		state = left ? BASHER_LEFT : BASHER_RIGHT;
		break;
	}

	case CLIMBER: {
		state = left ? CLIMBER_LEFT : CLIMBER_RIGHT;
		break;
	}

	case BUILDER: {
		state = left ? BUILDER_LEFT : BUILDER_RIGHT;
		break;
	}

	case BLOCKER: {
		state = BLOCKER_STATE;
		break;
	}
	}
}






