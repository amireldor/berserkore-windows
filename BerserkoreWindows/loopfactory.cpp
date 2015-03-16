#include "loopfactory.hpp"
#include "mainloopbase.hpp"
#include "gameloop.hpp"
#include "welcomeloop.hpp"
#include "deathloop.hpp"

using namespace bk;

LoopFactory::LoopPointer LoopFactory::create(LoopType what, YAML::Node *n_config, sf::RenderWindow *n_window, PubSub *n_pubsub, ResourcePointer n_resource)
{
	switch(what)
	{
		case WELCOME:
			return LoopPointer(new WelcomeScreenLoop(n_config, n_window, n_pubsub, n_resource));
		case DEATH:
			return LoopPointer(new DeathScreenLoop(n_config, n_window, n_pubsub, n_resource));
		case GAME:
		default:
			return LoopPointer(new GameMainLoop(n_config, n_window, n_pubsub, n_resource));
	}
}
