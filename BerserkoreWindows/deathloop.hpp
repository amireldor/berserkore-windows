#ifndef DEATHLOOP_HPP
#define DEATHLOOP_HPP

#include "mainloopbase.hpp"
#include <boost/shared_ptr.hpp>
#include "effecttext.hpp"

namespace bk
{

struct DeathScreenLoop : public MainLoopBase
{
	DeathScreenLoop(YAML::Node *n_config, sf::RenderWindow *n_window, PubSub *n_pubsub, ResourcePointer);
	void prepare();
	void processEvent(sf::Event);
	bool update();
	void draw();

	static const float TEXT_PULSE_TIME;

private:
	sf::Sprite background_sprite;
	EffectText text;
	sf::Color color1, color2;
	bool keep_going;
};

}// namespace bk

#endif

