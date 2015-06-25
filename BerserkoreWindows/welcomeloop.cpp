#include "welcomeloop.hpp"
#include "loopfactory.hpp"

using namespace bk;

WelcomeScreenLoop::WelcomeScreenLoop(YAML::Node *n_config, sf::RenderWindow *n_window, PubSub *n_pubsub, ResourcePointer n_resource) :
	MainLoopBase(n_config, n_window, n_pubsub, n_resource),
	keep_going(true)
{
}

void WelcomeScreenLoop::prepare()
{
	MainLoopBase::prepare();
	const YAML::Node &settings = (*config)["welcome"];

	background_sprite.setTexture( *resources->getTexture(settings["hello_image"].as<std::string>()) );

	text.setFont( *resources->getFont(settings["font"].as<std::string>()) );
	text.setCharacterSize(settings["size"].as<unsigned int>());
	text.setString(settings["message"].as<std::string>());
	sf::Uint8 r, g, b;
	r = settings["color"][0].as<unsigned int>();
	g = settings["color"][1].as<unsigned int>();
	b = settings["color"][2].as<unsigned int>();
	text.setColor(sf::Color(r, g, b));
	sf::Vector2f view = window->getView().getCenter();
	sf::FloatRect rect = text.getLocalBounds();
	text.setOrigin(rect.width/2, rect.height/2);
	text.setPosition(view.x, view.y);

	change_rotation_cooldown.set_maximum(settings["rotation_cooldown"].as<float>());
}

void WelcomeScreenLoop::processEvent(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		keep_going = false;
		this->next_loop = bk::LoopFactory::create(bk::LoopFactory::GAME, config, window, pubsub, resources);
	}
}

bool WelcomeScreenLoop::update()
{
	const YAML::Node &settings = (*config)["welcome"];

	const float movement = settings["movement_delta"].as<float>();
	const float rotation_delta = settings["rotation_delta"].as<float>();

	text.move(
		(-movement+2*movement*this->rand_zero_to_one()) * frameTime.asSeconds(),
		(-movement+2*movement*this->rand_zero_to_one()) * frameTime.asSeconds()
	);

	change_rotation_cooldown.update(frameTime.asSeconds());
	if (change_rotation_cooldown.is_ready())
	{
		change_rotation_cooldown.lock();
		text.setRotation( -rotation_delta + 2*rotation_delta*rand_zero_to_one() );
	}

	return keep_going;
}

void WelcomeScreenLoop::draw()
{
	window->clear();

	window->draw(background_sprite);
	window->draw(text);
}
