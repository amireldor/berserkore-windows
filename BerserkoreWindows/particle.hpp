/**
 * particle.hpp
 * Dirt, blood, fire, any particle system thing are particles. They are in this
 * file and the corresponding cpp is their behavior (in `update`).
 */
#ifndef _PARTICLE_HPP_
#define _PARTICLE_HPP_

#include <yaml-cpp/yaml.h>
#include "commondata.hpp"
#include "actor.hpp"
#include "animationseq.hpp"

namespace bk
{

class Particle : public Actor
{
public:
	Particle();
	virtual ~Particle();
	void update();

	float ix, iy;
	float iiy;

	float ttl; // Time To Live until we dispappear

private:

};

} // namespace bk

#endif
