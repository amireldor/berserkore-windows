#include "soundstack.hpp"

using namespace bk;

SoundStack::SoundStack()
	: limit_sounds_num_to(8)
{
}

SoundStack::SoundStack(int max_sounds)
	: limit_sounds_num_to(max_sounds)
{
}

void SoundStack::clearFront()
{
	while (	sound_deque.begin() != sound_deque.end()
		&& sound_deque.begin()->getStatus() != sf::Sound::Playing)
	{
		sound_deque.pop_front();
	}
}

void SoundStack::play(const sf::SoundBuffer& buffer, float pitch, float volume)
{
	clearFront();

	if (limit_sounds_num_to > 0)
	{
		int num_to_remove = sound_deque.size() - limit_sounds_num_to;
		while (num_to_remove-- >= 0)
		{
			sound_deque.pop_front();
		}
	}

 	sound_deque.push_back(sf::Sound(buffer));
	sound_deque.back().setPitch(pitch);
	sound_deque.back().setVolume(volume);
	sound_deque.back().play();
}

void SoundStack::releaseAll()
{
	sound_deque.clear();
}