#include "SoundManager.h"
#include <iostream>


SoundManager::SoundManager()
{
	uniqueID = 1;
}


SoundManager::~SoundManager()
{
}

bool SoundManager::init() {
	buffers[MUSIC1].loadFromFile("sounds/lemmings1.ogg");

	buffers[EXPLODE].loadFromFile("sounds/EXPLODE.WAV");

	buffers[DIE].loadFromFile("sounds/DIE.WAV");

	buffers[MUSIC2].loadFromFile("sounds/lemmings2.ogg");

	buffers[LETSGO].loadFromFile("sounds/LETSGO2.WAV");

	return true;
}

const int SoundManager::play(Sound sound, bool loop) {
	if (sound >= NUM_SOUNDS || sound < 0)
		return 0;
	std::cout << "playing" << sound << std::endl;
	SoundInfo soundInfo = { uniqueID, sf::Sound(buffers[sound])};
	sounds[sound].push_back(soundInfo);
	sounds[sound].back().sound.setVolume(100);
	sounds[sound].back().sound.setLoop(loop);
	sounds[sound].back().sound.play();
	++uniqueID;
	return sounds[sound].back().id;
}

bool SoundManager::stop(Sound sound, int id) {
	if (sound >= NUM_SOUNDS || sound < 0)
		return false;
	for (int i = 0; i < sounds[sound].size(); ++i) {
		if (sounds[sound].at(i).id == id) {
			sounds[sound].at(i).sound.stop();
		}
	}
	return true;
}

bool SoundManager::pause(Sound sound, int id) {
	if (sound >= NUM_SOUNDS || sound < 0)
		return false;
	for (int i = 0; i < sounds[sound].size(); ++i) {
		if (sounds[sound].at(i).id == id) {
			sounds[sound].at(i).sound.pause();
		}
	}
	return true;
}

void SoundManager::update() {
	for (int i = 0; i < NUM_SOUNDS; ++i) {
		while (!sounds[i].empty() && sounds[i].front().sound.getStatus() == sf::SoundSource::Status::Stopped) {
			sounds[i].pop_front();
		}
	}
}

bool SoundManager::isPlaying(Sound sound, int id) {
	if (sound >= NUM_SOUNDS || sound < 0)
		return false;
	bool ret = false;
	for (int i = 0; i < sounds[sound].size(); ++i) {
		if (sounds[sound].at(i).id == id) {
			ret = sounds[sound].at(i).sound.getStatus() == sf::SoundSource::Status::Playing;
		}
	}
	return ret;
}

bool SoundManager::isStopped(Sound sound, int id) {
	if (sound >= NUM_SOUNDS || sound < 0)
		return false;
	bool ret = false;
	for (int i = 0; i < sounds[sound].size(); ++i) {
		if (sounds[sound].at(i).id == id) {
			ret = sounds[sound].at(i).sound.getStatus() == sf::SoundSource::Status::Stopped;
		}
	}
	return ret;
}