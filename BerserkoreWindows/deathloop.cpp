#include "deathloop.hpp"
#include "loopfactory.hpp"

using namespace bk;

const float DeathScreenLoop::TEXT_PULSE_TIME = 0.372f; // should be in config.yml

DeathScreenLoop::DeathScreenLoop(YAML::Node *n_config, sf::RenderWindow *n_window, PubSub *n_pubsub, ResourcePointer n_resource) :
	MainLoopBase(n_config, n_window, n_pubsub, n_resource),
	keep_going(true)
{
}

void DeathScreenLoop::prepare()
{
	MainLoopBase::prepare();
	const YAML::Node &settings = (*config)["death"];

	background_sprite.setTexture(*resources->getTexture(settings["background"].as<std::string>()));

	text.setFont(*resources->getFont(settings["font"].as<std::string>()));
	text.setCharacterSize(settings["size"].as<unsigned int>());
	text.setString(settings["message"].as<std::string>());

	sf::Uint8 r, g, b;
	r = (sf::Uint8)settings["color"][0].as<unsigned int>();
	g = (sf::Uint8)settings["color"][1].as<unsigned int>();
	b = (sf::Uint8)settings["color"][2].as<unsigned int>();
	color1 = sf::Color(r, g, b);
	r = (sf::Uint8)settings["color2"][0].as<unsigned int>();
	g = (sf::Uint8)settings["color2"][1].as<unsigned int>();
	b = (sf::Uint8)settings["color2"][2].as<unsigned int>();
	color2 = sf::Color(r, g, b);

	text.setColor(color1);
	sf::Vector2f world_size = window->getView().getSize();
	sf::FloatRect rect = text.getLocalBounds();
	text.setOrigin(rect.width, rect.height);
	text.setPosition(world_size.x - 20, world_size.y - 10);

}

void DeathScreenLoop::processEvent(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		switch (event.key.code)
		{
			case sf::Keyboard::T:
				this->next_loop = LoopFactory::create(LoopFactory::GAME, config, window, pubsub, resources);
				// lack of break intentional (keep_going=false)
			case sf::Keyboard::Escape:
			case sf::Keyboard::C:
				keep_going = false;
		}
	}
}

bool DeathScreenLoop::update()
{
	text.update(frameTime.asSeconds());

	if (text.countEffects() == 0)
	{
		sf::Color new_color;
		if (text.getColor() == color1)
		{
			new_color = color2;
		}
		else {
			new_color = color1;
		}

		EffectBasePointer awesome_effect = EffectBasePointer(new ColorChange(&text, TEXT_PULSE_TIME, new_color));
		text.addEffect(awesome_effect);
	}

	return keep_going;
}

void DeathScreenLoop::draw()
{
	window->clear();

	window->draw(background_sprite);
	window->draw(text);
}
