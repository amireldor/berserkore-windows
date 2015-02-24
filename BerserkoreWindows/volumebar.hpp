#ifndef VOLUMEBAR_HPP
#define VOLUMEBAR_HPP

#include <SFML/Graphics.hpp>

namespace bk
{

class VolumeBar : public sf::Drawable
{
public:
	VolumeBar(const sf::FloatRect&);

	static const float CURRENT; // tell the update function to get the volume itself
	void value(float n_value = CURRENT); // should be like SFML: 0 - 100

	void appear(); // reset the hiding/fading-out
	void update(); // update the shapes so they scale right, and stuff

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	float volume_value;
	float alpha;
	sf::Clock appearance_clock;
	sf::RectangleShape background, foreground;
};

}// namespace bk

#endif

