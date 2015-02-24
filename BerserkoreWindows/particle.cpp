#include <boost/random/uniform_real_distribution.hpp>
#include "particle.hpp"
#include "resourcemanager.hpp"
#include "gameloop.hpp"

using namespace bk;

Particle::Particle()
 : ix(0), iy(0), iiy(0), ttl(1)
{
}

Particle::~Particle()
{
}

void
Particle::update()
{
	moveWithFPS(ix, iy);
	iy += iiy * getFps().asSeconds();
	ttl -= 1.0f * getFps().asSeconds();

	if (ttl <= 0)
	{
		shouldRemove();
	}

	/*sf::Vector2f pos = this->getPosition();
	if (pos.y > */
}
