#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Sprite.h"


Sprite *Sprite::createSprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program)
{
	Sprite *quad = new Sprite(quadSize, sizeInSpritesheet, spritesheet, program);

	return quad;
}


Sprite::Sprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program)
{
	float vertices[24] = {0.f, 0.f, 0.f, 0.f, 
												quadSize.x, 0.f, sizeInSpritesheet.x, 0.f, 
												quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y, 
												0.f, 0.f, 0.f, 0.f, 
												quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y, 
												0.f, quadSize.y, 0.f, sizeInSpritesheet.y};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = program->bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program->bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
	texture = spritesheet;
	shaderProgram = program;
	currentAnimation = -1;
	pos = glm::vec2(0.f);
	stop_render = 0;
	explosion = 0;
	death = 0;
}

int Sprite::update(int deltaTime)
{
	int frames = 0;

	if(currentAnimation >= 0)
	{
		timeAnimation += deltaTime;
		while(timeAnimation > animations[currentAnimation].millisecsPerKeyframe)
		{

			if (currentAnimation == 6) { //DEATH
				if (currentKeyframe == 15) { stop_render = 1; death = 1; }
			}
			if (currentAnimation == 7) { //EXPLOSION
				if (currentKeyframe == 15) { stop_render = 1; death = 1; }
				else if (currentKeyframe == 7) { explosion = 1; }
			}

			timeAnimation -= animations[currentAnimation].millisecsPerKeyframe;
			currentKeyframe = (currentKeyframe + 1) % animations[currentAnimation].keyframeDispl.size();

			if (currentAnimation == 2) {
				if (fallinganim_start && currentKeyframe == 0 ) currentKeyframe = 4;
				else if (currentKeyframe == 11) fallinganim_start = true;
			}
			else if (currentAnimation == 3) {
				if (fallinganim_start && currentKeyframe == 0) currentKeyframe = 4;
				else if (currentKeyframe == 11) fallinganim_start = true;
			}
			
			frames++;
		}
		texCoordDispl = animations[currentAnimation].keyframeDispl[currentKeyframe];
	}

	if (stop_render) {
		pos = glm::vec2(392, 192);
	}

	return frames;
}

void Sprite::render() const
{

	//if (!stop_render) {
		glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.f));
		shaderProgram->setUniformMatrix4f("modelview", modelview);
		shaderProgram->setUniform2f("texCoordDispl", texCoordDispl.x, texCoordDispl.y);
		glEnable(GL_TEXTURE_2D);
		shaderProgram->setTextureUnit("tex", 0);
		glActiveTexture(GL_TEXTURE0);
		texture->use();
		glBindVertexArray(vao);
		glEnableVertexAttribArray(posLocation);
		glEnableVertexAttribArray(texCoordLocation);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisable(GL_TEXTURE_2D);
	//}
}

void Sprite::free()
{
	glDeleteBuffers(1, &vbo);
}

void Sprite::setNumberAnimations(int nAnimations)
{
	animations.clear();
	animations.resize(nAnimations);
}

void Sprite::setAnimationSpeed(int animId, int keyframesPerSec)
{
	if(animId < int(animations.size()))
		animations[animId].millisecsPerKeyframe = 1000.f / keyframesPerSec;
}

void Sprite::addKeyframe(int animId, const glm::vec2 &displacement)
{
	if(animId < int(animations.size()))
		animations[animId].keyframeDispl.push_back(displacement);
}

void Sprite::changeAnimation(int animId)
{
	if(animId < int(animations.size()))
	{
		if ((animId == 2) || (animId == 3) ) fallinganim_start = 0;
		currentAnimation = animId;
		currentKeyframe = 0;
		timeAnimation = 0.f;
		texCoordDispl = animations[animId].keyframeDispl[0];
	}
}

bool Sprite::explosionKeyframe() {
	if (explosion) {
		explosion = 0;
		return true;
	}
	return false;
}

bool Sprite::deathKeyframe() {
	return death;
}

int Sprite::animation() const
{
	return currentAnimation;
}

void Sprite::setPosition(const glm::vec2 &newPos)
{
	pos = newPos;
}

glm::vec2 Sprite::position() const
{
	return pos;
}

glm::vec2 &Sprite::position()
{
	return pos;
}




