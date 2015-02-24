/**
 * A nice banner on map with level=0 that tells the player what to do
 */
#ifndef GO_HPP
#define GO_HPP

#include <yaml-cpp/yaml.h>
#include <boost/any.hpp>
#include "actor.hpp"
#include "listener.hpp"

namespace bk
{

class Go : public Actor, public Listener
{
public:
	Go(YAML::Node *config);

	void update();
	void onNotify(const std::string &message, boost::any data);
private:
	float counter_f; // for wobbling effect
	float radians_inc;
};

}

#endif
