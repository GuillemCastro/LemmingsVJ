#ifndef _LEMMING_INCLUDE
#define _LEMMING_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"


// Lemming is basically a Sprite that represents one lemming. As such it has
// all properties it needs to track its movement and collisions.

enum LemmingPower {

	NONE, CLIMBER, UMBRELLA, EXPLOADER, BLOCKER,  BUILDER,  BASHER, MINER, DIGGER, WALKER
};

enum Direction {
	LEFT, RIGHT, UP, DOWN
};

class Lemming
{

public:

	~Lemming() {
		if (sprite != NULL)
			delete sprite;
	}
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void setMapMask(VariableTexture *mapMask);
	void setBridges(VariableTexture* bridges);
	void setPower(LemmingPower power);
	bool insideCollisionBox(int x, int y);
	bool isAlive();
	LemmingPower getPower();
	
private:
	int collisionFloor(int maxFall);
	bool collision(Direction direction);
	bool collision(Direction direction, int bottom);
	bool collisionStopper(Direction direction);
	int collisionCeilling(int max);
	
	
private:
	enum LemmingState
	{
		WALKING_LEFT_STATE, WALKING_RIGHT_STATE, FALLING_LEFT_STATE, FALLING_RIGHT_STATE, 
		DIGGER_STATE, BASHER_LEFT_STATE, BASHER_RIGHT_STATE, CLIMBER_LEFT_STATE, CLIMBER_RIGHT_STATE, BUILDER_LEFT_STATE, 
		BUILDER_RIGHT_STATE, BLOCKER_STATE, UMBRELLA_RIGHT_STATE, UMBRELLA_LEFT_STATE, DEAD_STATE, EXPLOAD_STATE
	};

	const int blockerMask = 1;
	LemmingState state;
	LemmingPower power;
	Texture spritesheet;
	Sprite *sprite;
	VariableTexture *mask;
	VariableTexture *bridges;
	int fallHight;
	int builderCount;
	bool initalized;
	bool ignoreBlocker;
	bool alive, playDie;

};


#endif // _LEMMING_INCLUDE


