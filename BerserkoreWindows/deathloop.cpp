#include "deathloop.hpp"


using namespace bk;

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
	r = settings["color"][0].as<unsigned int>();
	g = settings["color"][1].as<unsigned int>();
	b = settings["color"][2].as<unsigned int>();
	text.setColor(sf::Color(r, g, b));
	sf::Vector2f world_size = window->getView().getSize();
	sf::FloatRect rect = text.getLocalBounds();
	text.setOrigin(rect.width, rect.height);
	text.setPosition(world_size.x - 20, world_size.y - 10);

	EffectBasePointer awesome_effect = EffectBasePointer(new ColorChange(&text, 1.f, sf::Color(255,255,0)));
	text.addEffect(awesome_effect);

}

void DeathScreenLoop::processEvent(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		keep_going = false;
	}
}

bool DeathScreenLoop::update()
{
	text.update(frameTime.asSeconds());
	return keep_going;
}

void DeathScreenLoop::draw()
{
	window->clear();

	window->draw(background_sprite);
	window->draw(text);
}
