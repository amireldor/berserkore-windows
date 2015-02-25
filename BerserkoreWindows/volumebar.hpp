#ifndef VOLUMEBAR_HPP
#define VOLUMEBAR_HPP

#include <SFML/Graphics.hpp>
#include <yaml-cpp/yaml.h>

#include "resourcemanager.hpp"

namespace bk
{

class VolumeBar : public sf::Drawable
{
public:
	VolumeBar(YAML::Node*, const sf::FloatRect&, ResourcePointer);

	static const float CURRENT; // tell the update function to get the volume itself
	void value(float n_value = CURRENT); // should be like SFML: 0 - 100

	void appear(); // reset the hiding/fading-out
	void update(); // update the shapes so they scale right, and stuff
	
	ResourcePointer resources;
	sf::Font fontamir;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	float volume_value;
	float alpha;
	sf::Clock appearance_clock;
	sf::RectangleShape background, foreground;

	sf::Text label; // so people will understand that it's a volume bar

	YAML::Node* config;
};

}// namespace bk

#endif

