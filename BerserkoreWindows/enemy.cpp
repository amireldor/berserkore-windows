#include <cmath>
#include "enemy.hpp"
#include "hero.hpp"
#include "gameloop.hpp"
#include "bomb.hpp"

using namespace bk;
EnemySoldier::EnemySoldier(YAML::Node *config, CommonGameData *data)
  : Soldier(config, data), shooting(false)
{
	this->team = Team::RED;

	// make shooting start or stop in a random time
	// the const time start_stop is set up later when this first rand
	// is complete
	fire_start_stop_cooldown.set_maximum(
		data->game->rand_zero_to_one() * (*config)["enemy"]["fire_start_stop_cooldown"].as<float>()
	);
	fire_start_stop_cooldown.lock();
}

EnemySoldier::~EnemySoldier()
{
}

void EnemySoldier::update()
{
	Soldier::update();

	sf::Vector2f hero_pos = data->hero->getPosition();

	// put on ground
	putOnGround();

	// look at hero
	sf::Vector2f pos = getPosition();
	if (pos.x < hero_pos.x)
	{
		look(Actor::RIGHT);
	} else {
		look(Actor::LEFT);
	}

	// now fire your rifle, maybe
	if (fire_start_stop_cooldown.update(data->game->frameTime.asSeconds()))
	{
		// reset its maximum, as it's not the normal value upon restart
		// (see EnemySoldier())
		fire_start_stop_cooldown.set_maximum(
			(*config)["enemy"]["fire_start_stop_cooldown"].as<float>()
		);
		fire_start_stop_cooldown.lock();
		if ((*config)["enemy"]["fire_chance"].as<float>() >= data->game->rand_zero_to_one())
		{
			shooting = !shooting;
			if (shooting)
			{
				// shoot immediately
				rifle_cooldown.reset();
			}
		}
	}

	if (shooting && rifle_cooldown.is_ready())
	{
		fire();
	}
}

void EnemySoldier::onNotify(const std::string &message, boost::any data)
{
	if (message == "grenade:hits_ground")
	{
		Bomb *bomb = boost::any_cast<Bomb*>(data);
		sf::Vector2f ground_zero = bomb->getPosition();

		if ( fabsf(getPosition().x - ground_zero.x) < bomb->getWidth()/2 )
		{
			// the bomb hit me!
			shouldRemove();
			// TODO:
			// limbs flying and screams of agony and sounds of gore
			this->data->game->bloodExplosion(this->getPosition());
		}
	}
}

void EnemySoldier::putOnGround()
{
	sf::Vector2f pos = getPosition();
	pos.y = (*data->ground)[static_cast<unsigned int>(pos.x)] * data->game->map_height;
	pos.y -= data->game->main_texture_subrect_selector.frame.y / 2; // put exactly on ground, considering the height of my image
	setPosition(pos);
}

void EnemySoldier::shouldRemove(bool should)
{
	// TODO: DRY, dupliocated shit in bomb
	data->pubsub->unsubscribe_from_all(shared_from_this());
	Actor::shouldRemove(should);
}
