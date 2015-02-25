#ifndef SOUNDSTACK_H
#define SOUNDSTACK_H

#include <deque>
#include <SFML/Audio.hpp>

namespace bk
{

class SoundStack
{
public:
	/**
	 * default max_sounds is provided (I think it's 8)
	 */
	SoundStack();
	/**
	 * max_sounds can be negative for 'no-limit' on sounds number
	 */
	SoundStack(int max_sounds);

	void play(const sf::SoundBuffer&, float pitch = 1, float volume = 100);

	/**
	 * pops the non_playing items from the beginning of the stack.
	 * called automatically, it should have been protected, but life's good.
	 */
	void clearFront();

	void releaseAll();

private:
	std::deque<sf::Sound> sound_deque;
	/**
	 * limit number of items in the deque. set negative for 'no-limit'.
	 */
	int limit_sounds_num_to;

};

}

#endif
