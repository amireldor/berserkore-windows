#include "loopfactory.hpp"

using namespace bk;

LoopFactory::LoopPointer LoopFactory::create(LoopType what, YAML::Node *n_config, sf::RenderWindow *n_window, PubSub *n_pubsub)
{
	switch(what)
	{
		case WELCOME:
			return LoopPointer(new WelcomeScreenLoop(n_config, n_window, n_pubsub));
		case GAME:
			return LoopPointer(new GameMainLoop(n_config, n_window, n_pubsub));
		default:
			return create(GAME, n_config, n_window, n_pubsub);
	}
}
