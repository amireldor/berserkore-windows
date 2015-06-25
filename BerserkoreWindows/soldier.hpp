#ifndef SOLDIER_H
#define SOLDIER_H

#include <yaml-cpp/yaml.h>
#include "actor.hpp"
#include "cooldown.hpp"
#include "commondata.hpp"

namespace bk
{

class Soldier : public Actor
{
public:
	Soldier();
	Soldier(YAML::Node* config, CommonGameData *data);

	CommonGameData *data;
	int team;

	void fire();

	virtual void update();

protected:
	YAML::Node* config;

	float shot_speed;
	CoolDown rifle_cooldown;
};

}

#endif
