#include <cmath>
#include "happyhealth.hpp"

using namespace bk;

HappyHealth::HappyHealth(sf::Sprite* n_target, float n_rate)
 :	target(n_target),
	x(0.f),
	shown(true),
	rate(n_rate)
{
}

void HappyHealth::show(bool status)
{
	this->shown = status;
}

void HappyHealth::update(float dt)
{
	// place and tweak position
	sf::Vector2f pos = target->getPosition();
	pos.x -= this->getTextureRect().width*0.5;
	pos.y -= this->getTextureRect().height*1.6;
	this->setPosition(pos);

	// pulsate beautifully! in GREEN!
	float alpha;
	if (shown)
	{
		x += rate * dt;
		alpha = cosf(x)/2.0f + 0.5f;
	} else {
		// "reset"
		x = 0;
		alpha = 0;
	}
	this->setColor(sf::Color(0, 255, 0, alpha*255.0f));
}
