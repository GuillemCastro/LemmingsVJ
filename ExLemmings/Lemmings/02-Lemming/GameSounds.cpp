#include "GameSounds.h"
#include <iostream>


GameSounds::GameSounds()
{
}


GameSounds::~GameSounds()
{
}

bool GameSounds::init() {
	bool ret = false;
	for (int i = 0; i < NUM_SOUNDS; ++i) {
		buffers[i] = new sf::SoundBuffer();
	}
	ret = ret || buffers[MUSIC1]->loadFromFile("sounds/lemmings1.wav");
	//sounds[MUSIC1] = sf::Sound(buffers[MUSIC1]);

	ret = ret || buffers[EXPLODE]->loadFromFile("sounds/EXPLODE.WAV");
	//sounds[EXPLODE] = sf::Sound(buffers[EXPLODE]);
	return ret;
}

bool GameSounds::play(Sound sound, bool loop) {
	if (sound >= NUM_SOUNDS || sound < 0)
		return false;
	std::cout << "playing" << sound << std::endl;
	sf::Sound* soundToPlay = new sf::Sound(*buffers[sound]);
	soundToPlay->setVolume(100);
	soundToPlay->setLoop(loop);
	soundToPlay->play();
	sounds.push_back(soundToPlay);
	return true;
}

bool GameSounds::stop(Sound sound) {
	if (sound >= NUM_SOUNDS || sound < 0)
		return false;
	//sounds[sound].stop();
	return true;
}

bool GameSounds::pause(Sound sound) {
	if (sound >= NUM_SOUNDS || sound < 0)
		return false;
	//sounds[sound].pause();
	return true;
}