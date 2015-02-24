#ifndef BOMB_H
#define BOMB_H

#include <string>
#include <yaml-cpp/yaml.h>
#include <boost/any.hpp>
#include "resourcemanager.hpp"
#include "actor.hpp"
#include "ground.hpp"
#include "listener.hpp"
#include "commondata.hpp"
#include "animationseq.hpp" // for AnimFrame

namespace bk
{

class Bomb : public Actor, public Listener
{
public:
	static const AnimFrame textureGrenade;
	static const AnimFrame textureBomb;
	static const std::string soundNameGrenade;
	static const std::string soundNameBomb;

	Bomb(
		YAML::Node *config, CommonGameData data, float width, float depth,
		float inc_x=0, float ini_inc_y=0, float inc_rot=0,
		AnimFrame textureFrame=textureGrenade,
		std::string n_sound_name=soundNameGrenade
	);
	CommonGameData data;

	void update();

	void onNotify(const std::string &message, boost::any data);

	float getWidth() const { return damage_width; }
	float getDepth() const { return damage_depth; }

	std::string getSoundName() const { return sound_name; } // FIXME I accidently C++ with normal return const& and stuff

private:
	float damage_width, damage_depth;
	float ix, iy, irot;
	std::string sound_name;
};

}

#endif
