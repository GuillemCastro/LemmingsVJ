#ifndef _DOOR_INCLUDE
#define _DOOR_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"



class Door
{

public:

	~Door() {
		if (sprite != NULL)
			delete sprite;
	}
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, bool entryDoor);
	void update(int deltaTime);
	void render();

private:
	Texture spritesheet;
	Sprite *sprite;
};


#endif // _DOOR_INCLUDE


