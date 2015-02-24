#ifndef SHOT_H
#define SHOT_H

#include <yaml-cpp/yaml.h>
#include "actor.hpp"
#include "commondata.hpp"

namespace bk
{

class Shot : public Actor
{
public:
	Shot(YAML::Node *config, CommonGameData data);
	virtual ~Shot();

	int team;

	/**
	 * Move forward according to `direction`, NOT with FPS.
	 */
	void advance(float amount);

	/**
	 * This considers FPS as well.
	 */
	void advanceWithFPS(float amount);

	void update();

	YAML::Node *config;
	CommonGameData data;
};

}

#endif
