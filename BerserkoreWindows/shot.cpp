#include "shot.hpp"
#include "ground.hpp"
#include "gameloop.hpp"
#include "pubsub.hpp"
#include "hero.hpp"

using namespace bk;

Shot::Shot(YAML::Node *config, CommonGameData data)
  : config(config), data(data)
{
}

Shot::~Shot()
{
}

void
Shot::advance(float amount)
{
	if (direction == RIGHT)
	{
		move(amount, 0);
	} else {
		move(-amount, 0);
	}
}

void
Shot::advanceWithFPS(float amount)
{
	advance(amount * getFps().asSeconds());
}

void
Shot::update()
{
	sf::Vector2f pos = getPosition();
	/** for this shot
	 * check if in ground, and erase if is
	 */
	try
	{
		float ground_height = (*data.ground)[static_cast<unsigned int>(pos.x)] * data.game->map_height;
		if (pos.y >= ground_height)
		{
			shouldRemove();
			return;
		}
	} catch (GroundException &e) {
		// probably out of screen...
		shouldRemove();
		return;
	}
	// yay we handled ground/shot collision. continue.

	const float shot_radius = data.game->main_texture_subrect_selector.frame.x/2; // TODO: move to config

	// this is the estimated distnace the shot will fly in this frame
	const float shot_distance_per_frame = getSpeed() * data.game->frameTime.asSeconds();
	/** if we are not in the red team, for each enemy
	 * check if shot is colliding in the current frame (according to speed)
	 * set erase flag if needed
	 */
	// TODO
	// Make a shot<==>soldier collision function to adhere to the DRY priciple...
	// (there's a duplicate check for enemy soldiers and for hero below)
	if (team != Team::RED)
	{
		// let's begin
		ActorList::iterator soldier = data.game->enemysoldiers.begin();
		while (soldier != data.game->enemysoldiers.end())
		{
			sf::Vector2f soldier_pos = (*soldier)->getPosition();

			// check y
			if (abs(soldier_pos.y - pos.y) <= shot_radius)
			{
				// in-range, now check x with shot's dynamic path

				// get shot flight x endpoints
				float start_x, end_x;
				if (getDirection() == RIGHT)
				{
					start_x = pos.x - shot_radius/2;
					end_x = pos.x + shot_distance_per_frame + shot_radius/2;
				} else {
					start_x = pos.x - shot_distance_per_frame - shot_radius/2;
					end_x = pos.x + shot_radius/2;
				}

				if (soldier_pos.x > start_x && soldier_pos.x < end_x)
				{
					(*soldier)->shouldRemove();
					data.pubsub->publish("enemy:tango_down", soldier_pos);
					shouldRemove();
					break;
				}
			}

			soldier++;
		}
	} else {
		// bullet is from the RED team, check collision with Lousy Man
		sf::Vector2f hero_pos = data.hero->getPosition();

		// check y
		if (!data.hero->isDead() && abs(hero_pos.y - pos.y) <= shot_radius)
		{
			// in-range, now check x with shot's dynamic path

			// get shot flight x endpoints
			float start_x, end_x;
			if (getDirection() == RIGHT)
			{
				start_x = pos.x - shot_radius/2;
				end_x = pos.x + shot_distance_per_frame + shot_radius/2;
			} else {
				start_x = pos.x - shot_distance_per_frame - shot_radius/2;
				end_x = pos.x + shot_radius/2;
			}

			if (hero_pos.x > start_x && hero_pos.x < end_x)
			{
				data.pubsub->publish("hero:got_shot");
				shouldRemove();
			}
		}
	}
	if (should_remove) return; // don't continue stam

	/**
	 * advance a bit
	 */
	advanceWithFPS(speed);
	pos = getPosition();
	if (pos.x < 0 || pos.x >= getMapWidth())
	{
		shouldRemove();
	}
}
