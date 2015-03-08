#ifndef LOOP_FACTORY_H
#define LOOP_FACTORY_H

#include <boost/shared_ptr.hpp>
#include "gameloop.hpp"
#include "welcomeloop.hpp"
#include "resourcemanager.hpp"

namespace bk
{

class MainLoopBase;

class PubSub;

class LoopFactory
{
public:
	enum LoopType
	{
		NOTHING,
		WELCOME,
		GAME,
		DEATH,
		DEFAULT = WELCOME
	};

	typedef boost::shared_ptr<bk::MainLoopBase> LoopPointer;

	LoopPointer create(LoopType what, YAML::Node *n_config, sf::RenderWindow *n_window, PubSub *n_pubsub, ResourcePointer n_resource);

};

} // namespace bk

#endif
