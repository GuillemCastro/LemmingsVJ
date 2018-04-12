#include "SoundManager.h"
#include <iostream>


SoundManager::SoundManager()
{
}


SoundManager::~SoundManager()
{
}

bool SoundManager::init() {
	buffers[MUSIC1].loadFromFile("sounds/lemmings1.wav");
	//sounds[MUSIC1] = sf::Sound(buffers[MUSIC1]);

	buffers[EXPLODE].loadFromFile("sounds/EXPLODE.WAV");

	buffers[DIE].loadFromFile("sounds/DIE.WAV");
	//sounds[EXPLODE] = sf::Sound(buffers[EXPLODE]);
	return true;
}

bool SoundManager::play(Sound sound, bool loop) {
	if (sound >= NUM_SOUNDS || sound < 0)
		return false;
	std::cout << "playing" << sound << std::endl;
	sounds[sound].push_back(sf::Sound(buffers[sound]));
	sounds[sound].back().setVolume(100);
	sounds[sound].back().setLoop(loop);
	sounds[sound].back().play();
	return true;
}

bool SoundManager::stop(Sound sound) {
	if (sound >= NUM_SOUNDS || sound < 0)
		return false;
	//sounds[sound].stop();
	return true;
}

bool SoundManager::pause(Sound sound) {
	if (sound >= NUM_SOUNDS || sound < 0)
		return false;
	//sounds[sound].pause();
	return true;
}

void SoundManager::update() {
	for (int i = 0; i < NUM_SOUNDS; ++i) {
		while (!sounds[i].empty() && sounds[i].front().getStatus() == sf::SoundSource::Status::Stopped) {
			sounds[i].pop_front();
		}
	}
}