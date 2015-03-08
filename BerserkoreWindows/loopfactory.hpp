#ifndef LOOP_FACTORY_H
#define LOOP_FACTORY_H

#include <boost/shared_ptr.hpp>
#include <SFML/Graphics.hpp>
#include <yaml-cpp/yaml.h>

#include "resourcemanager.hpp"

namespace bk
{

struct MainLoopBase;
class PubSub;

class LoopFactory
{
public:
	enum LoopType : unsigned int
	{
		NOTHING,
		WELCOME,
		GAME,
		DEATH,
		DEFAULT = WELCOME
	};

	typedef boost::shared_ptr<bk::MainLoopBase> LoopPointer;

	static LoopPointer create(LoopType what, YAML::Node *n_config, sf::RenderWindow *n_window, PubSub *n_pubsub, ResourcePointer n_resource);

};

} // namespace bk

#endif
