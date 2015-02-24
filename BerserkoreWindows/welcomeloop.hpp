#ifndef WELCOMELOOP_HPP
#define WELCOMELOOP_HPP

#include "mainloopbase.hpp"
#include "cooldown.hpp"

namespace bk
{

struct WelcomeScreenLoop : public MainLoopBase
{
	WelcomeScreenLoop(YAML::Node *n_config, sf::RenderWindow *n_window, PubSub *n_pubsub);
	void prepare();
	void processEvent(sf::Event);
	bool update();
	void draw();

private:
	sf::Sprite background_sprite;
	sf::Text text;
	bool keep_going;
	CoolDown change_rotation_cooldown;

};

}// namespace bk

#endif

