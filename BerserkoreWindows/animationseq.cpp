#include "animationseq.hpp"

using namespace bk;

AnimationSeq::AnimationSeq(
			boost::shared_ptr< std::vector<AnimFrame> > animation,
			unsigned int max_frames,
			float animation_speed,
			bool loop)
{
	sequence = animation;
	this->max_frames = max_frames;
	speed = animation_speed;
	looping = loop;
	current_frame = 0.f;
}

AnimFrame AnimationSeq::getCurrentFrame()
{
	return (*sequence)[ static_cast<unsigned int>(current_frame) ];
}

void AnimationSeq::setFrame(float frame)
{
	if (frame < 0.f) frame = 0.f;
	current_frame = frame;
}

void AnimationSeq::update(sf::Time fps)
{
	current_frame += speed * fps.asSeconds();
	if (current_frame >= max_frames) {
		// current_frame back to valid value
		if (looping) {
			unsigned int iterations =
				static_cast<unsigned int>
				(current_frame / max_frames);
			current_frame -= max_frames * iterations;
		} else {
			current_frame = (float)(max_frames - 1);
		}
	}
}
