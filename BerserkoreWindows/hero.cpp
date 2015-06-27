#include "hero.hpp"
#include "resourcemanager.hpp"
#include "gameloop.hpp"
#include "pubsub.hpp"

using namespace bk;

Hero::Hero(YAML::Node *config, CommonGameData *data, float speed)
  : Soldier(config, data), life(1.0f)
{
	setTexture(*data->resources->getTexture((*config)["main_texture"].as<std::string>()));

	setupAnimation(config);

	do_stand_anim();

	setOrigin((float)data->game->main_texture_subrect_selector.frame.x / 2,
		(float)data->game->main_texture_subrect_selector.frame.y / 2);

	grenade_cooldown.set_maximum((*config)["hero"]["grenade_cooldown"].as<float>());
	rifle_cooldown.set_maximum((*config)["hero"]["rifle_cooldown"].as<float>());
	shot_speed = (*config)["hero"]["shot_speed"].as<float>();

	look(RIGHT);
	this->speed = speed;
	this->team = Team::GREEN;
	this->walking = false;
}


Hero::~Hero()
{
	std::cout << "hero destructor" << std::endl;
}

void
Hero::setupAnimation(YAML::Node *config)
{
	if (walk_anim_frames == nullptr)
	{
		walk_anim_frames = boost::shared_ptr< std::vector<AnimFrame> >( new std::vector<AnimFrame> );
	}
	walk_anim_frames->clear();
	walk_anim_frames->push_back(AnimFrame(1, 0));
	walk_anim_frames->push_back(AnimFrame(2, 0));
	walk_anim_frames->push_back(AnimFrame(3, 0));
	walk_anim_frames->push_back(AnimFrame(4, 0));

	this->walk_anim_sequence = AnimationSeq(walk_anim_frames, 4, 14.34f);
	// FIXME previous line: move anim speed (3rd param) to config?
}

void
Hero::do_stand_anim()
{
	setTextureRect(data->game->main_texture_subrect_selector.rect(0, 0)); // 'stand' stance
}

void
Hero::do_throw_anim()
{
	setTextureRect(data->game->main_texture_subrect_selector.rect(0, 1)); // 'throw grenade' stance
}

bool
Hero::is_throwing_grenade() const
{
	return !grenade_cooldown.is_ready();
}

bool
Hero::is_walking() const
{
	return walking;
}

bool
Hero::try_throw_grenade()
{
	if (!isDead() && grenade_cooldown.is_ready())
	{
		grenade_cooldown.lock();
		//throw_sound_stack.play(throw_buffer);
		throw_sound.play();
		data->pubsub->publish("grenade:new");
		return true;
	}
	return false;
}

void
Hero::update()
{
	Soldier::update();

	if (isDead())
	{
		return;
	}

	// Grenade throwing and cooldown
	grenade_cooldown.update(getFps().asSeconds());

	// Walk animation and actual moving
	walking = false;
	if (
		sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4)
		)
	{
		moveWithFPS(-speed, 0);
		look(LEFT);
		walking = true;
	} else if (
		sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6)
		)
	{
		moveWithFPS(speed, 0);
		look(RIGHT);
		walking = true;
	}

	// choose sub-rect texture to show
	if (is_throwing_grenade())
	{
		do_throw_anim();
	} else if (walking) {
		walk_anim_sequence.update(getFps());
		// Prepare texture for drawing which is probably the next step after
		// this `update`.
		setTextureRect(
			data->game->main_texture_subrect_selector.rect(walk_anim_sequence.getCurrentFrame())
		);
	} else {
		do_stand_anim();
	}

	// Limit to map width
	sf::Vector2f position;
	position = getPosition();
	if (position.x < 0)
	{
		position.x = 0;
	} else if (position.x >= getMapWidth())
	{
		// new map
		position.x = 0;
		data->pubsub->publish("map:new");
	}

	// Put hero on the ground
	position.y = (*data->ground)[static_cast<int>(position.x)] * getMapHeight();
	position.y -= getTextureRect().height / 2;

	// Update position
	setPosition(position);

	// fire rifle
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		fire();
	}
}

void Hero::addLife(float howmuch)
{
	if (howmuch < 0) return;
	life += howmuch;
}

bool Hero::reduceLife(float howmuch)
{
	life -= howmuch;
	if (life <= .0f)
	{
		life = .0f;
		this->data->pubsub->publish("hero:dead");
	}
	return isDead();
}
