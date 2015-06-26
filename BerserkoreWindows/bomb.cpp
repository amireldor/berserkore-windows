#include "bomb.hpp"
#include "gameloop.hpp"
#include "pubsub.hpp"

using namespace bk;

// TODO: maybe these should not be const and should be in config
const AnimFrame Bomb::textureBomb = AnimFrame(2, 1);
const AnimFrame Bomb::textureGrenade = AnimFrame(1, 1);
const std::string Bomb::soundNameBomb = "bomb";
const std::string Bomb::soundNameGrenade = "grenade";

Bomb::Bomb(YAML::Node *config, CommonGameData *data,
	float width, float depth, float inc_x, float ini_inc_y, float inc_rot,
	AnimFrame textureFrame, std::string n_sound_name)
  : data(data), damage_width(width), damage_depth(depth), sound_name(n_sound_name)
{
	// set texture
	setTexture(*data->resources->getTexture((*config)["main_texture"].as<std::string>()));
	setTextureRect(data->game->main_texture_subrect_selector.rect(textureFrame));
	// set origin
	setOrigin((float)data->game->main_texture_subrect_selector.frame.x / 2,
		(float)data->game->main_texture_subrect_selector.frame.y / 2);

	ix = inc_x;
	iy = ini_inc_y;
	irot = inc_rot;
}

void
Bomb::update()
{
	Actor::update();
	moveWithFPS(ix, iy);
	rotate( irot * getFps().asSeconds() );

	iy += getGravity() * getFps().asSeconds();

	// check if we hit the ground
	sf::Vector2f pos = getPosition();
	if (pos.x < 0 || pos.x >= getMapWidth())
	{
		shouldRemove();
	}
	else
	{
		unsigned int death_height = (unsigned int)((*data->ground)[static_cast<unsigned int>(pos.x)] * getMapHeight());
		if (pos.y >= death_height)
		{
			// we hit the ground!
			data->pubsub->publish("grenade:hits_ground", this);
			shouldRemove();
		}
	}
}

void
Bomb::onNotify(const std::string &message, boost::any data)
{
	// TODO: THIS IS BAD AND UGLY
	/* TODO: remove from here. use some kind of `Action`s instead.
	 * The actions will be defined in the GameMainLoop and they will
	 * be niced here afterwards, separated game logic from game data
	 */
	if (message == "map:new")
	{
		shouldRemove();
	}
}

void Bomb::shouldRemove()
{
	// TODO: this is duplicated stuff. there's another actor/listener that does the same somewhere
	this->data->pubsub->unsubscribe_from_all(shared_from_this());
	Actor::shouldRemove();
}