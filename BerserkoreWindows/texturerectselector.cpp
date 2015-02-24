#include "texturerectselector.hpp"

using namespace bk;

TextureRectSelector::TextureRectSelector(sf::Vector2u frame)
  : frame(frame)
{
}

sf::IntRect TextureRectSelector::rect(AnimFrame frame)
{
	return this->rect(frame.x, frame.y);
}

sf::IntRect TextureRectSelector::rect(unsigned int x, unsigned int y)
{
	return sf::IntRect(
		x * frame.x, y * frame.y,
		frame.x, frame.y
	);
}
