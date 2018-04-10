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
	WALKING_LEFT, WALKING_RIGHT, UMBRELLA_RIGHT, UMBRELLA_LEFT, BLOCKING, DIGGING, DEATH, EXPLOSION, FALLING_RIGHT, FALLING_LEFT, BASHING_RIGHT, BASHING_LEFT
};

void Lemming::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	ignoreBlocker = 0; // when lemmings fall on top of a bloquer they must ignore it
	state = WALKING_RIGHT_STATE;
	power = NONE;
	initalized = 1;
	builderCount = 0;
	fallHight = 0;
	spritesheet.loadFromFile("images/lemming_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);

	float numAnim = 10.0f;
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, (1/numAnim)), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(numAnim);
	
	sprite->setAnimationSpeed(WALKING_RIGHT, 12);
	float y = 0.0f;
	for(int i=0; i<8; i++)
		sprite->addKeyframe(WALKING_RIGHT, glm::vec2(float(i) / 16, y));
		
	sprite->setAnimationSpeed(WALKING_LEFT, 12);
	y += 1 / numAnim;
	for(int i=0; i<8; i++)
		sprite->addKeyframe(WALKING_LEFT, glm::vec2(float(i) / 16, y));

	sprite->setAnimationSpeed(UMBRELLA_RIGHT, 10);
	y += 1 / numAnim;
	for (int i = 0; i<12; i++)
		sprite->addKeyframe(UMBRELLA_RIGHT, glm::vec2(float(i) / 16, y));

	sprite->setAnimationSpeed(UMBRELLA_LEFT, 10);
	y += 1 / numAnim;
	for (int i = 11; i>=0; i--) // Done because sprite is backwards
		sprite->addKeyframe(UMBRELLA_LEFT, glm::vec2(float(i) / 16, y));

	sprite->setAnimationSpeed(BLOCKING, 12);
	y += 1 / numAnim;
	for (int i = 0; i<16; i++)
		sprite->addKeyframe(BLOCKING, glm::vec2(float(i) / 16, y));

	sprite->setAnimationSpeed(DIGGING, 12);
	y += 1 / numAnim;
	for (int i = 0; i<8; i++)
		sprite->addKeyframe(DIGGING, glm::vec2(float(i) / 16, y));

	sprite->setAnimationSpeed(DEATH, 12);
	y += 1 / numAnim;
	for (int i = 0; i<16; i++)
		sprite->addKeyframe(DEATH, glm::vec2(float(i) / 16, y));

	sprite->setAnimationSpeed(EXPLOSION, 12);
	y += 1 / numAnim;
	for (int i = 0; i<16; i++)
		sprite->addKeyframe(EXPLOSION, glm::vec2(float(i) / 16, y));

	sprite->setAnimationSpeed(FALLING_RIGHT, 10);
	y += 1 / numAnim;
	for (int i = 0; i<4; i++)
		sprite->addKeyframe(FALLING_RIGHT, glm::vec2(float(i) / 16, y));

	sprite->setAnimationSpeed(FALLING_LEFT, 10);
	y += 1 / numAnim;
	for (int i = 3; i >= 0; i--) // Done because sprite is backwards
		sprite->addKeyframe(FALLING_LEFT, glm::vec2(float(i) / 16, y));
	
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
	case FALLING_RIGHT_STATE:
		if (power == UMBRELLA) {
			state = UMBRELLA_RIGHT_STATE;
			break;
		}

		fall = collisionFloor(2);
		if (fall > 0) {
			sprite->position() += glm::vec2(0, fall);
			fallHight += fall;
		}
		else {
			if (fallHight > 30) {
				sprite->changeAnimation(DEATH);
				state = DEAD_STATE;
			}
			else {
				if (power == BUILDER) {
					fallHight = 0;
					state = BUILDER_RIGHT_STATE;
				}
				else {
					fallHight = 0;
					sprite->changeAnimation(WALKING_RIGHT);
					state = WALKING_RIGHT_STATE;
					break;
				}
			}
		}
		break;
	case FALLING_LEFT_STATE:
		if (power == UMBRELLA) {
			state = UMBRELLA_LEFT_STATE;
			break;
		}

		fall = collisionFloor(2);
		if (fall > 0)
			sprite->position() += glm::vec2(0, fall);
		else {
			if (fallHight > 30) {
				sprite->changeAnimation(DEATH);
				state = DEAD_STATE;
			}
			else {
				if (power == BUILDER) {
					fallHight = 0;
					state = BUILDER_RIGHT_STATE;
				}
				else {
					fallHight = 0;
					sprite->changeAnimation(WALKING_RIGHT);
					state = WALKING_RIGHT_STATE;
					break;
				}
			}
		}
		break;
	case UMBRELLA_LEFT_STATE:
		fall = collisionFloor(2);
		if(fall > 0)
			sprite->position() += glm::vec2(0, 2);
		else {
			if (power == BUILDER) {
				state = BUILDER_LEFT_STATE;
			}
			else {
				sprite->changeAnimation(WALKING_LEFT);
				state = WALKING_LEFT_STATE;
			}
		}
		break;
	case UMBRELLA_RIGHT_STATE:
		fall = collisionFloor(2);
		if(fall > 0)
			sprite->position() += glm::vec2(0, 2);
		else {
			if (power == BUILDER) {
				state = BUILDER_RIGHT_STATE;
			}
			else {
				sprite->changeAnimation(WALKING_RIGHT);
				state = WALKING_RIGHT_STATE;
				break;
			}
		}
		break;
	case WALKING_LEFT_STATE:
		sprite->position() += glm::vec2(-1, -1);
		if(collision())
		{
			if (power == BASHER) {
				state = BASHER_LEFT_STATE;
			}
			else if (power == CLIMBER && collisionCeilling(3) > 1) {
				state = CLIMBER_LEFT_STATE;
			}
			else {
				sprite->position() -= glm::vec2(-1, -1);
				sprite->changeAnimation(WALKING_RIGHT);
				state = WALKING_RIGHT_STATE;
			}
		}
		else if (bridges->pixel(sprite->position().x + 7, sprite->position().y + 16) == 0) {
			fall = collisionFloor(3);
			if(fall > 0)
				sprite->position() += glm::vec2(0, 1);
			if(fall > 1)
				sprite->position() += glm::vec2(0, 1);
			if (fall > 2) {
				sprite->changeAnimation(FALLING_LEFT);
				state = FALLING_LEFT_STATE;
			}
		}
		break;
	case WALKING_RIGHT_STATE:
		sprite->position() += glm::vec2(1, -1);
		if(collision())
		{
			if (power == BASHER) {
				state = BASHER_RIGHT_STATE;
			}
			else if (power == CLIMBER && collisionCeilling(3) > 1) {
				cout << "collisionCeilling " << collisionCeilling(3) << endl;
				state = CLIMBER_RIGHT_STATE;
			}
			else {
				sprite->position() -= glm::vec2(1, -1);
				sprite->changeAnimation(WALKING_LEFT);
				state = WALKING_LEFT_STATE;
			}
		}
		else if (bridges->pixel(sprite->position().x + 7, sprite->position().y + 16) == 0) {
			fall = collisionFloor(3);
			if (fall > 0)
				sprite->position() += glm::vec2(0, 1);
			if (fall > 1)
				sprite->position() += glm::vec2(0, 1);
			if (fall > 2) {
				sprite->changeAnimation(FALLING_RIGHT);
				state = FALLING_RIGHT_STATE;
			}		
		}
		break;
	case DIGGER_STATE: {
		fall = collisionFloor(2);
		if (fall > 0) {
			state = UMBRELLA_RIGHT_STATE;
			power = NONE;
			sprite->changeAnimation(FALLING_RIGHT);
		}
		else {
			if (sprite->animation() != DIGGING) {
				sprite->changeAnimation(DIGGING);
			}
			sprite->position() += glm::vec2(0, 1);

			for (int x = max(0.0f, sprite->position().x + 4.0f); x <= min(mask->width() - 1.0f, sprite->position().x + 10.0f); ++x) {
				mask->setPixel(x + 120, sprite->position().y + 15, 0);
			}
		}
		break;
	}

	case BASHER_LEFT_STATE: {
		fall = collisionFloor(2);
		if (fall > 0) {
			state = FALLING_LEFT_STATE;
			power = NONE;
			sprite->changeAnimation(FALLING_LEFT);
		}
		else {
			if (sprite->animation() != BASHING_LEFT) {
				//change to bashing left
			}
			sprite->position() += glm::vec2(-1.0f, 0.0f);
			for (int y = max(0.0f, sprite->position().y + 8.0f); y <= min(mask->height() - 1.0f, sprite->position().y + 16.0f); ++y) {
				mask->setPixel(sprite->position().x + 120 + 8.f, y, 0);
			}
		}
		break;
	}

	case BASHER_RIGHT_STATE: {
		fall = collisionFloor(2);
		if (fall > 0) {
			state = FALLING_RIGHT_STATE;
			power = NONE;
			sprite->changeAnimation(FALLING_RIGHT);
		}
		else {
			if (sprite->animation() != BASHING_RIGHT) {
				//change to bashing RIGHT
			}
			sprite->position() += glm::vec2(1.0f, 0.0f);
			for (int y = max(0.0f, sprite->position().y + 8.0f); y <= min(mask->height() - 1.0f, sprite->position().y + 16.0f); ++y) {
				mask->setPixel(sprite->position().x + 120.0f + 7.9f, y, 0);
			}
		}
		break;
	}
	case BLOCKER_STATE: {
		fall = collisionFloor(2);
		if (fall > 0) {
			state = FALLING_RIGHT_STATE;
			sprite->changeAnimation(FALLING_RIGHT_STATE);
		}
		else {
			if (sprite->animation() != BLOCKING) {
				sprite->changeAnimation(BLOCKING);
			}

			for (int x = max(0.0f, sprite->position().x + 2.0f); x <= min(mask->width() - 1.0f, sprite->position().x + 12.0f); ++x) {
				for (int y = max(0.0f, sprite->position().y + 5.0f); y <= min(mask->height() - 1.0f, sprite->position().y + 15.0f); ++y) {
					if (mask->pixel(x + 120, y) == 0.0f) mask->setPixel(x + 120, y, blockerMask); // This will be an issue when we have the escavator as he might be able to escavate though a lemming
				}
			}
		}
		break;
	}
	case CLIMBER_LEFT_STATE: {
		sprite->position() += glm::vec2(0.f, -1.f);
		int ceilling = collisionCeilling(1);
		fall = collisionFloor(2);
		if (!collision()) {
			state = WALKING_LEFT_STATE;
			sprite->changeAnimation(WALKING_LEFT);
		}
		if (ceilling > 0) {
			if (fall > 0) {
				state = FALLING_LEFT_STATE;
				sprite->changeAnimation(FALLING_LEFT);
			}
			else {
				state = WALKING_LEFT_STATE;
				if (sprite->animation() != WALKING_LEFT)
					sprite->changeAnimation(WALKING_LEFT);
			}
		}
		break;
	}
	case CLIMBER_RIGHT_STATE: {
		sprite->position() += glm::vec2(0.f, -1.f);
		int ceilling = collisionCeilling(1);
		fall = collisionFloor(2);
		if (!collision()) {
			state = WALKING_RIGHT_STATE;
			sprite->changeAnimation(WALKING_RIGHT);
		}
		if (ceilling > 0) {
			if (fall > 0) {
				state = FALLING_RIGHT_STATE;
				sprite->changeAnimation(FALLING_RIGHT);
			}
			else {
				state = WALKING_RIGHT_STATE;
				if (sprite->animation() != WALKING_RIGHT)
					sprite->changeAnimation(WALKING_RIGHT);
			}
		}
		break;
	}
	case BUILDER_LEFT_STATE: {
		if (builderCount >= 50) {
			builderCount = 0;
			power = NONE;
			state = WALKING_LEFT_STATE;
			sprite->changeAnimation(WALKING_LEFT);
		}
		else if (collision()) {
			builderCount = 0;
			power = NONE;
			state = WALKING_RIGHT_STATE;
			sprite->changeAnimation(WALKING_RIGHT);
		}
		else {
			sprite->position() += glm::vec2(-1.f, -1.f);
			bridges->setPixel(sprite->position().x + 7, sprite->position().y + 16, 255);
			builderCount += 1;
		}
		break;
	}
	case BUILDER_RIGHT_STATE: {
		if (builderCount >= 50) {
			builderCount = 0;
			power = NONE;
			state = WALKING_RIGHT_STATE;
			sprite->changeAnimation(WALKING_RIGHT);
		}
		else if (collision()) {
			builderCount = 0;
			power = NONE;
			state = WALKING_LEFT_STATE;
			sprite->changeAnimation(WALKING_LEFT);
		}
		else {
			bridges->setPixel(sprite->position().x + 7, sprite->position().y + 16, 255);
			sprite->position() += glm::vec2(1.f, -1.f);
			builderCount += 1;
		}
		break;
	}
	case DEAD_STATE:
		if (sprite->deathKeyframe()) alive = 0;
		break;
	case EXPLOAD_STATE:
		if (sprite->animation() != EXPLOSION) {
			sprite->changeAnimation(EXPLOSION);
		}
		if (sprite->explosionKeyframe()) {
			float r = 10.0f;
			glm::vec2 pos = sprite->position();
			pos.x += 8;
			pos.y += 14;
			for (int x = max(0.0f, sprite->position().x- 5.0f); x <= min(mask->width() - 1.0f, sprite->position().x + 20.0f); ++x) {
				for (int y = max(0.0f, sprite->position().y - 4.0f); y <= min(mask->height() - 1.0f, sprite->position().y + 25.0f); ++y) {
					glm::vec2 aux(x, y);
					float dist = distance(pos, aux);
					if (dist < r) {
						mask->setPixel(x + 120, y, 0);
					}
				}			
			}
			state = DEAD_STATE;
		}
	}
}

void Lemming::render()
{
	alive = 1;
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

	if (collisionCeilling(3) == 1) {
		cout << "collisionCeilling is 1" << endl;
		return 1;
	}
	
	posBase += glm::ivec2(7, 16);
	while((fall < maxFall) && !bContact)
	{
		if((mask->pixel(posBase.x, posBase.y+fall) == 0) && (mask->pixel(posBase.x+1, posBase.y+fall) == 0) &&
			(bridges->pixel(posBase.x, posBase.y + fall) == 0) && (bridges->pixel(posBase.x + 1, posBase.y + fall) == 0))
			fall += 1;
		else if ((mask->pixel(posBase.x, posBase.y + fall) == blockerMask) && (mask->pixel(posBase.x + 1, posBase.y + fall) == blockerMask)) {
			ignoreBlocker = 1;
			fall += 1;
		}
		else
			bContact = true;
	}

	return fall;
}

bool Lemming::collision()
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 15);
	if ((mask->pixel(posBase.x, posBase.y) == 0) && (mask->pixel(posBase.x + 1, posBase.y) == 0) && (mask->pixel(posBase.x + 1, posBase.y - 1) == 0)) {
		if (ignoreBlocker) ignoreBlocker = 0;
		return false; // lemming can no longer fit though 1 pixel blocs, they now must have at least be 2 pixels with the mask set to 0 no not detect collision
	}
	else if (((mask->pixel(posBase.x, posBase.y) == blockerMask) || (mask->pixel(posBase.x + 1, posBase.y) == blockerMask) || (mask->pixel(posBase.x + 1, posBase.y - 1) == blockerMask)) && (ignoreBlocker))
		return false;  // so there are no bugs when lemmings fall on blockers
	return true;
}

void Lemming::setPower(LemmingPower power) {

	if (this->power == CLIMBER) {
		return; // The climber is a permanent skill!
	}

	this->power = power;

	bool left = false;
	if (state == WALKING_LEFT_STATE || state == FALLING_LEFT_STATE || state == BASHER_LEFT_STATE
		|| state == CLIMBER_LEFT_STATE || state == BUILDER_LEFT_STATE || state == UMBRELLA_LEFT_STATE) {
		left = true;
	}

	switch (power) {
	case DIGGER: {
		state = DIGGER_STATE;
		break;
	}

	/*case BASHER: {
		state = left ? BASHER_LEFT : BASHER_RIGHT; basher only bash when collision() == true
		break;
	}

	case CLIMBER: {
		state = left ? CLIMBER_LEFT : CLIMBER_RIGHT;
		break;
	}*/

	case BUILDER: {
		if (state != UMBRELLA_LEFT_STATE && state != UMBRELLA_RIGHT_STATE)
			state = left ? BUILDER_LEFT_STATE : BUILDER_RIGHT_STATE;
		break;
	}

	case BLOCKER: {
		state = BLOCKER_STATE;
		break;
	}
	case WALKER: {
		this->power = NONE;
		state = left ? WALKING_LEFT_STATE : WALKING_RIGHT_STATE;
		break;
	}
	case EXPLOADER: {
		this->power = NONE;
		state = EXPLOAD_STATE;
		break;
	}
	}
}

int Lemming::collisionCeilling(int max) {

	bool bContact = false;
	int fall = 0;
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement

	posBase += glm::ivec2(7, 0);
	while ((fall < max) && !bContact)
	{
		if ((mask->pixel(posBase.x, posBase.y - fall) == 0))
			fall += 1;
		else if ((mask->pixel(posBase.x, posBase.y - fall) == blockerMask) && (mask->pixel(posBase.x + 1, posBase.y - fall) == blockerMask)) {
			ignoreBlocker = 1;
			fall += 1;
		}
		else
			bContact = true;
	}

	return fall;
}

void Lemming::setBridges(VariableTexture* bridges) {
	this->bridges = bridges;
}

bool Lemming::isAlive() {
	if (initalized && !alive) {
		return 0;
	}
	return true;
}

