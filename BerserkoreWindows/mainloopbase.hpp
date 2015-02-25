#ifndef MAINLOOP_HPP
#define MAINLOOP_HPP

#include <yaml-cpp/yaml.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/shared_ptr.hpp>

#include "texturerectselector.hpp"
#include "resourcemanager.hpp"

namespace bk
{

class PubSub;

struct MainLoopBase
{
	MainLoopBase(YAML::Node *n_config, sf::RenderWindow *n_window, PubSub *n_pubsub, ResourcePointer n_resource);

	virtual void prepare(); // you should call it if you subclass it. really.

	boost::random::mt19937 random_gen;
	static boost::random::uniform_real_distribution<float> zero_to_one_dist;
	float rand_zero_to_one();

	TextureRectSelector main_texture_subrect_selector;

	YAML::Node *config;
	boost::shared_ptr<ResourceManager> resources;

	sf::RenderWindow *window; // set it up yourself!
	virtual void processEvent(sf::Event) = 0;
	virtual bool update() = 0; // return `false` if you are done with your loop
	virtual void draw() = 0;

	sf::Time frameTime;

protected:
	PubSub *pubsub;

};

}// namespace bk

#endif
