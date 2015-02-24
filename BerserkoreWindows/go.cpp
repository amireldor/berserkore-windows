#include "go.hpp"
#include "defs.hpp"

using namespace bk;

Go::Go(YAML::Node *config)
  : counter_f(0.0f)
{
	this->speed = (*config)["go"]["speed"].as<float>();
	setPosition(
		(*config)["go"]["pos_percent"][0].as<float>() * getMapWidth(),
		(*config)["go"]["pos_percent"][1].as<float>() * getMapHeight()
	);
	this->radians_inc = (*config)["go"]["deg_inc"].as<float>() * M_PI / 180;
}

void
Go::update()
{
	counter_f += radians_inc * getFps().asSeconds();
	move(speed * sinf(counter_f) * getFps().asSeconds(), 0);
}

void
Go::onNotify(const std::string &message, boost::any data)
{
	if (message == "map:new")
	{
		this->shouldRemove();
	}
}
