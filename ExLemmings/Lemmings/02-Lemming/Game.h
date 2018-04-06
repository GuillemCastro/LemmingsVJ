#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "Text.h"
#include "TexturedQuad.h"
#include <iostream>
#include "MenuScene.h"


// Game is a singleton (a class with a single instance) that represents our whole application

enum GameScene {
	MENU, SCENE1, SCENE2, SCENE3
};

class Game
{

public:
	Game() {}
	
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);

	void pause();
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

	void changeScene(GameScene state);

private:

	void initShaders();

	bool bPlay;                       // Continue to play game?
	bool paused;					  // Game paused?
	Scene* scene;                      // Scene to render
	MenuScene* menuScene;
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	int mouseX, mouseY;               // Mouse position
	bool bLeftMouse, bRightMouse;     // Mouse button states
	Text pausedText;
	TexturedQuad* pausedQuad;
	Texture plainTexture;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	glm::mat4 projection;
	GameScene state;

};


#endif // _GAME_INCLUDE


