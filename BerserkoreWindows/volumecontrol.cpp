#include "volumecontrol.hpp"
#include <SFML/Audio.hpp>

using namespace bk;

const float VolumeControl::DEFAULT_STEP = 100/6.0f;
float VolumeControl::pre_mute_vol = 100;

void VolumeControl::volUp(float howmuch)
{
	float n_vol = sf::Listener::getGlobalVolume();
	n_vol += howmuch;
	if (n_vol > 100)
	{
		n_vol = 100;
	}

	sf::Listener::setGlobalVolume(n_vol);
}

void VolumeControl::volDown(float howmuch)
{
	float n_vol = sf::Listener::getGlobalVolume();
	n_vol -= howmuch;
	if (n_vol < 0)
	{
		n_vol = 0;
	}

	sf::Listener::setGlobalVolume(n_vol);
}

void VolumeControl::muteToggle()
{
	float n_vol = sf::Listener::getGlobalVolume();

	if (n_vol > 0)
	{
		// not muted, mute it!
		pre_mute_vol = n_vol;
		sf::Listener::setGlobalVolume(0.f);
	} else {
		// muted, restore volume
		sf::Listener::setGlobalVolume(pre_mute_vol);
	}

}
