/**
 *
 * happyhealth.hpp
 * Happy health is some indicator that the hero's health is increasing.
 *
 * It's a happy pulsating '+' sign whenever health is +1. YAY~!
 *
 */


#ifndef HAPPYHEALTH_HPP
#define HAPPYHEALTH_HPP

#include <SFML/Graphics.hpp>

namespace bk
{

class HappyHealth : public sf::Sprite
{
public:

	HappyHealth(sf::Sprite* n_target, float n_rate);

	void show(bool status = true);

	void update(float dt);

private:

	sf::Sprite* target;

	float x; // the value inside cosf()
	bool shown;
	float rate;
};

}

#endif // HAPPYHELATH_HPP
