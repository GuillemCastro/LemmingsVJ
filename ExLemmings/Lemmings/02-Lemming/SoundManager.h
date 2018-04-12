#pragma once

#include <SFML\Audio.hpp>
#include <deque>

enum Sound {
	MUSIC1 = 0,
	EXPLODE,
	DIE,

	NUM_SOUNDS
};

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	bool init();

	void update();

	bool play(Sound sound, bool loop);

	bool pause(Sound sound);

	bool stop(Sound sound);

private:
	sf::SoundBuffer buffers[NUM_SOUNDS];
	std::deque<sf::Sound> sounds[NUM_SOUNDS];
};

