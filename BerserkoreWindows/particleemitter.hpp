#ifndef PARTICLEEMITTER_HPP
#define PARTICLEEMITTER_HPP

#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <yaml-cpp/yaml.h>
#include "commondata.hpp"
#include "actor.hpp"
#include "animationseq.hpp"

namespace bk
{

/**
 * Emits particles, and dies (with shouldRemove)
 */
class ParticleEmitter : public Actor // although not really visible
{
public:

	struct ParticleParameters {
		float ini_ix, ini_iy;
		float min_rand_ix, max_rand_ix;
		float min_rand_iy, max_rand_iy;
		float iiy;
		float ttl;
		AnimFrame anim_frame; // the sub-picture in the common texure
	};
	typedef std::map<std::string, ParticleParameters> ParticleParametersContainer;

	ParticleEmitter(YAML::Node *config, CommonGameData *data, ActorList* list, ParticleParameters n_params);
	virtual ~ParticleEmitter();
	virtual void update();

	float emitter_rate;
	float emitter_life;

	static const float DONT_DIE; // value for life = <infinite>

	void createParticle(const ParticleParameters &params);
	void createParticles(unsigned int how_many, const ParticleParameters &params);

	static ParticleParametersContainer presets;
	static void initPresets(const YAML::Node& config);
	static ParticleParameters& preset(std::string name);

private:
	YAML::Node *config;
	CommonGameData *data;
	ActorList* particles_list;

	ParticleParameters params;

	sf::Clock clock;
	float particles_to_create;

};

} // namespace bk

#endif

