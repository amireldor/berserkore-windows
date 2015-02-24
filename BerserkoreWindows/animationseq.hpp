/**
 * Animation Sequence
 *
 * An array of unsigned ints that represents the frame numbers for animations.
 * We use (x, y) for each frame.
 * Updates to the animation are done according to FPS.
 */

#ifndef ANIMATIONSEQ_H
#define ANIMATIONSEQ_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include <SFML/Graphics.hpp>

namespace bk
{

typedef sf::Vector2u AnimFrame;

class AnimationSeq
{
public:
	AnimationSeq() {};
	AnimationSeq(
		boost::shared_ptr< std::vector<AnimFrame> > animation,
		unsigned int max_frames,
		float animation_speed,
		bool loop = true);

	/*void setSequence(boost::shared_ptr< std::vector<AnimFrame> > newSeq)
	{
		sequence = newSeq;
	}
	void setMaxFrames(unsigned int new_max_frames) { max_frames = new_max_frames; ;
	void setSpeed(float new_speed) { speed = new_speed; }
	void setLooping(bool lop) { loopnig = loop ;*/

	void setSpeed(float new_speed) { speed = new_speed; }

	AnimFrame getCurrentFrame();
	void update(sf::Time fps);

	void setFrame(unsigned int frame) { current_frame = (float)frame; };
	void setFrame(float frame);

private:
	float current_frame;
	float speed;
	unsigned int max_frames;
	boost::shared_ptr< std::vector<AnimFrame> > sequence;
	bool looping;
};

}

#endif
