#include "volumebar.hpp"
#include <SFML/Audio.hpp>

using namespace bk;

const float VolumeBar::CURRENT = -1;

VolumeBar::VolumeBar(YAML::Node *n_config, const sf::FloatRect &rect, ResourcePointer n_resource):
	resources(n_resource)
{
	config = n_config;

	value(CURRENT);

	// setup shapes
	background.setSize(sf::Vector2f(rect.width, rect.height));
	foreground.setSize(sf::Vector2f(rect.width, rect.height));

	background.setPosition(rect.left, rect.top);
	foreground.setPosition(rect.left, rect.top);

	background.setFillColor(sf::Color::Black);
	background.setOutlineColor(sf::Color::Black);
	background.setOutlineThickness(1);
	foreground.setFillColor(sf::Color::Green);

	alpha = 0; // start hidden

	label.setFont(*(resources->getFont((*config)["main_font"].as<std::string>())));
	label.setString("volume");
	label.setColor(sf::Color(0, 130, 0));
	label.setCharacterSize(8); // TODO: this should move to config.yml
	sf::FloatRect label_rect = label.getLocalBounds();
	label.setOrigin(label_rect.width / 2.f, label_rect.height / 2.f);
	label.setPosition(rect.left+rect.width / 2.0f, rect.top);
}

void VolumeBar::value(float n_value)
{
	if (n_value == CURRENT)
	{
		n_value = sf::Listener::getGlobalVolume();
	}

	volume_value = n_value;
}

void VolumeBar::appear()
{
	appearance_clock.restart();
	alpha = 255;
}

void VolumeBar::update()
{
	sf::Time time_since_appear = appearance_clock.getElapsedTime();

	// compute the alpha
	if (alpha > 0 && time_since_appear.asSeconds() > 1)
	{
		// hack hack
		const float fade_time = 0.377f;
		float factor = fade_time - (time_since_appear.asSeconds() - 1);
		if (factor < 0) factor = 0;
		factor /= fade_time;
		alpha = factor * 255;
	}

	// set the alpha
	sf::Color foreground_color = foreground.getFillColor();
	sf::Color background_color = background.getFillColor();
	foreground_color.a = (sf::Uint8)alpha;
	background_color.a = (sf::Uint8)alpha;
	foreground.setFillColor(foreground_color);
	background.setFillColor(background_color);
	background.setOutlineColor(background_color);
	sf::Color label_color = label.getColor();
	label_color.a = (sf::Uint8)alpha; // update alpha
	label.setColor(label_color);

	// set foreground scale
	foreground.setScale(volume_value/100.f, 1);
}

void VolumeBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

	if (alpha <= 0) return;
	target.draw(background);
	target.draw(foreground);
	target.draw(label);
}
