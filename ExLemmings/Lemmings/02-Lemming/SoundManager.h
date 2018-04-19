#pragma once

#include <SFML\Audio.hpp>
#include <deque>

enum Sound {
	MUSIC1 = 0,
	EXPLODE,
	DIE,
	MUSIC2,
	LETSGO,
	MUSIC3,

	NUM_SOUNDS
};

struct SoundInfo {
	int id;
	sf::Sound sound;
};

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	static SoundManager& instance() {
		static SoundManager sm;
		return sm;
	}

	bool init();

	void update();

	const int play(Sound sound, bool loop);

	bool pause(Sound sound, int id);

	bool stop(Sound sound, int id);

	bool isPlaying(Sound sound, int id);

	bool isStopped(Sound sound, int id);

private:

	int uniqueID;

private:
	sf::SoundBuffer buffers[NUM_SOUNDS];
	std::deque<SoundInfo> sounds[NUM_SOUNDS];
};

