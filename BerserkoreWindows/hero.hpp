/**
 * The main hero
 */

#ifndef HERO_H
#define HERO_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <yaml-cpp/yaml.h>
#include "soldier.hpp"
#include "animationseq.hpp"
#include "cooldown.hpp"
#include "commondata.hpp"
#include "soundstack.hpp"

namespace bk
{

struct GameMainLoop;

class Hero : public Soldier
{
public:
	Hero(YAML::Node *config, CommonGameData data, float speed);
	virtual ~Hero();

	void setupAnimation(YAML::Node *config);

	void update();

	void do_stand_anim();
	void do_throw_anim();

	/**
	 * Will return if thrown or not. May not be thrown because already
	 * throwing a grenade, or because no grenades left(?).
	 */
	bool try_throw_grenade();
	bool is_throwing_grenade() const;
	bool is_walking() const;

	float getLife() const { return life; }
	/**
	 * NO! you can't add negative values!
	 */
	void addLife(float howmuch);
	/**
	 * returns true if dead
	 */
	bool reduceLife(float howmuch);
	bool isDead() const { return life <= 0.0f; }

private:
	AnimationSeq walk_anim_sequence;
	boost::shared_ptr< std::vector<AnimFrame> > walk_anim_frames;

	CoolDown grenade_cooldown;
	bool walking;

	float life;

	SoundStack throw_sound_stack; // grenade throwing
	sf::SoundBuffer throw_buffer; // why the hell are the sound buffers in `resources` shared_ptrs?
};

}

#endif
