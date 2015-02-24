/**
 * Common data to give constructors that should access important stuff such as
 * the Hero  instance or Ground/GameMainLoop.
 */
#ifndef COMMONDATA_H
#define COMMONDATA_H

#include <boost/shared_ptr.hpp>

namespace bk
{

class Hero;
class Ground;
class PubSub;
class ResourceManager;
struct GameMainLoop;

struct CommonGameData
{
	boost::shared_ptr<Hero> hero;
	boost::shared_ptr<Ground> ground;
	PubSub* pubsub;
	boost::shared_ptr<ResourceManager> resources;
	boost::shared_ptr<GameMainLoop> game;
};

}

#endif
