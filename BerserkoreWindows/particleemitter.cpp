#include "particleemitter.hpp"
#include "particle.hpp"
#include "resourcemanager.hpp"
#include "gameloop.hpp"

using namespace bk;

ParticleEmitter::ParticleParametersContainer ParticleEmitter::presets;

ParticleEmitter::ParticleEmitter(YAML::Node *config, CommonGameData data, ActorList* list, ParticleParameters n_params):
	config(config), data(data), particles_list(list),
	params(n_params)
{
}

ParticleEmitter::~ParticleEmitter()
{
}

const float ParticleEmitter::DONT_DIE = -1;

void ParticleEmitter::update()
{
		// Update and die
	if (emitter_life != DONT_DIE)
	{
		emitter_life -= 1 * getFps().asSeconds();
		if (emitter_life <= 0)
		{
			shouldRemove();
		}
	}

	if (!dont_keep())
	{
		// YAY create particles.
		// how much time passed? how many particles to create according to rate?
		sf::Time time_passed = clock.getElapsedTime();
		particles_to_create += emitter_rate * time_passed.asSeconds();
		clock.restart();
		unsigned int int_particles_to_create = static_cast<unsigned int>(particles_to_create);

		/*ParticleParameters params;
		params.ini_ix = 150;
		params.ini_iy = -78;
		params.iiy = getGravity();
		params.min_rand_ix = -20;
		params.max_rand_ix = +20;
		params.min_rand_iy = -50;
		params.max_rand_iy = +8;
		params.ttl = 3;*/
		createParticles(int_particles_to_create, params);

		particles_to_create -= int_particles_to_create;
	}
}

void ParticleEmitter::createParticle(const ParticleParameters &params)
{
	// Create particle
	boost::shared_ptr<Particle> particle (new Particle);
	particle->setTexture(*data.resources->getTexture((*config)["particles_texture"].as<std::string>()));
	particle->setTextureRect(data.game->main_texture_subrect_selector.rect(params.anim_frame));
	particle->setPosition(getPosition());
	// TODO set subrect

	boost::random::uniform_real_distribution<float>
		ix_dist(params.min_rand_ix, params.max_rand_ix);
	boost::random::uniform_real_distribution<float>
		iy_dist(params.min_rand_iy, params.max_rand_iy);

	particle->ix = params.ini_ix + ix_dist(data.game->random_gen);
	particle->iy = params.ini_iy + iy_dist(data.game->random_gen);
	particle->iiy = params.iiy;
	particle->ttl = params.ttl;

	// Add it
	particles_list->push_back(particle);
}

void ParticleEmitter::createParticles(unsigned int how_many, const ParticleParameters &params)
{
	for (unsigned int i = 0; i < how_many; ++i)
	{
		createParticle(params);
	}
}

void ParticleEmitter::initPresets(const YAML::Node &config)
{
	YAML::Node particle_presets = config["particles"]["presets"];

	YAML::Node::iterator iter = particle_presets.begin();
	while(iter != particle_presets.end())
	{
		std::string name = iter->first.as<std::string>();
		try
		{
			// parse the parameters
			ParticleParameters params;

			YAML::Node curr = iter->second;
			params.ini_ix = curr["ini_ix"].as<float>();
			params.ini_iy = curr["ini_iy"].as<float>();
			params.min_rand_ix = curr["min_rand_ix"].as<float>();
			params.max_rand_ix = curr["max_rand_ix"].as<float>();
			params.min_rand_iy = curr["min_rand_iy"].as<float>();
			params.max_rand_iy = curr["max_rand_iy"].as<float>();
			params.ttl = curr["ttl"].as<float>();
			params.anim_frame = AnimFrame(curr["frame"][0].as<unsigned int>(), curr["frame"][1].as<unsigned int>());

			if (!curr["iiy"].IsDefined())
			{
				params.iiy = config["gravity"].as<float>();
			} else {
				params.iiy = curr["iiy"].as<float>();
			}

			presets[name] = params;
		}
		catch (YAML::Exception & e)
		{
			// ignore
		}

		iter++;
	}
}

ParticleEmitter::ParticleParameters& ParticleEmitter::preset(std::string name)
{
	return presets[name];
}
