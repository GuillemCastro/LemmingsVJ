#pragma once

#include <SFML\Audio.hpp>
#include <deque>

enum Sound {
	MUSIC1 = 0,
	EXPLODE,

	NUM_SOUNDS
};

class GameSounds
{
public:
	GameSounds();
	~GameSounds();

	bool init();

	bool play(Sound sound, bool loop);

	bool pause(Sound sound);

	bool stop(Sound sound);

private:
	sf::SoundBuffer* buffers[NUM_SOUNDS];
	std::deque<sf::Sound*> sounds;
};

