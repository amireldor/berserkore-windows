#include "soldier.hpp"
#include "shot.hpp"
#include "listener.hpp"
#include "pubsub.hpp"

using namespace bk;

Soldier::Soldier()
{
}

Soldier::Soldier(YAML::Node* config, CommonGameData data)
  :  data(data), config(config)
{
	rifle_cooldown.set_maximum((*config)["soldier"]["rifle_cooldown"].as<float>());
	shot_speed = (*config)["soldier"]["shot_speed"].as<float>();
}

void
Soldier::fire()
{
	if (rifle_cooldown.is_ready())
	{
		rifle_cooldown.lock();

		boost::shared_ptr<Shot> newshot(new Shot(config, data));

		sf::Vector2f pos = getPosition();
		int fac = 1;
		if (getDirection() == LEFT) fac = -1;

		newshot->look(getDirection());

		pos += sf::Vector2f(fac * (*config)["soldier"]["size"][0].as<int>() / 2.f, 0);
		newshot->setPosition(pos);

		newshot->speed = shot_speed;
		newshot->team = team;

		data.pubsub->publish("shot:new",  newshot);

	}
}

void
Soldier::update()
{
	Actor::update();
	rifle_cooldown.update(getFps().asSeconds());
}
