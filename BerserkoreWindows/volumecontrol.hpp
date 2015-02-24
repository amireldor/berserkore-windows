#ifndef VOLUMECONTROL_HPP
#define VOLUMECONTROL_HPP

namespace bk
{

class VolumeControl
{

public:

	static void volUp(float howmuch=DEFAULT_STEP);
	static void volDown(float howmuch=DEFAULT_STEP);
	static void muteToggle();

	static const float DEFAULT_STEP;

private:

	static float pre_mute_vol;

};

} // namespace bk

#endif
