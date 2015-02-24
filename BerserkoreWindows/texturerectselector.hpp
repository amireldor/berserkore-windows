#ifndef TEXTURERECTSELECTOR_H
#define TEXTURERECTSELECTOR_H

#include <SFML/Graphics.hpp>
#include "animationseq.hpp" // for bk::AnimFrame

namespace bk {

class TextureRectSelector
{
public:
	TextureRectSelector() {}; // empty doing nothing

	/**
	 * @param frame The size in pixels of each frame
	 */
	TextureRectSelector(sf::Vector2u frame);

	sf::IntRect rect(bk::AnimFrame frame);
	sf::IntRect rect(unsigned int x, unsigned int y = 0);

	sf::Vector2u frame;
};

}

#endif
