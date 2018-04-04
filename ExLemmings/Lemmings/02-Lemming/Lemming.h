#ifndef _LEMMING_INCLUDE
#define _LEMMING_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"


// Lemming is basically a Sprite that represents one lemming. As such it has
// all properties it needs to track its movement and collisions.

enum LemmingPower {

	NONE, DIGGER, BASHER, CLIMBER, BUILDER, BLOCKER
};

class Lemming
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	bool isInit();
	void setMapMask(VariableTexture *mapMask);
	void setPower(LemmingPower power);
	bool insideCollisionBox(int x, int y);
	
private:
	int collisionFloor(int maxFall);
	bool collision();
	
private:
	enum LemmingState
	{
		WALKING_LEFT_STATE, WALKING_RIGHT_STATE, FALLING_LEFT_STATE, FALLING_RIGHT_STATE, 
		DIGGER_STATE, BASHER_LEFT, BASHER_RIGHT, CLIMBER_LEFT, CLIMBER_RIGHT, BUILDER_LEFT, 
		BUILDER_RIGHT, BLOCKER_STATE, UMBRELLA_RIGHT_STATE, UMBRELLA_LEFT_STATE
	};

	const int blockerMask = 1;
	LemmingState state;
	LemmingPower power;
	Texture spritesheet;
	Sprite *sprite;
	VariableTexture *mask;
	bool initalized;
	bool ignoreBlocker;

};


#endif // _LEMMING_INCLUDE


