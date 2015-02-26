#include "mainloopbase.hpp"

using namespace bk;

boost::random::uniform_real_distribution<float> MainLoopBase::zero_to_one_dist(0.0f, 1.0f);

MainLoopBase::MainLoopBase(YAML::Node *n_config, sf::RenderWindow *n_window, PubSub *n_pubsub, ResourcePointer n_resource):
	config(n_config),
	window(n_window),
	pubsub(n_pubsub),
	resources(n_resource)
{
}

float MainLoopBase::rand_zero_to_one()
{
	return zero_to_one_dist(random_gen);
}

void MainLoopBase::prepare()
{
	random_gen.seed(std::time(NULL));
	main_texture_subrect_selector.frame = sf::Vector2u(
		(*config)["images"]["hero"]["frame"]["size"][0].as<unsigned int>(),
		(*config)["images"]["hero"]["frame"]["size"][1].as<unsigned int>()
	);
}
