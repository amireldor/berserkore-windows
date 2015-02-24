#include "actor.hpp"

using namespace bk;

int Actor::map_width, Actor::map_height;
sf::Time* Actor::fps;
float Actor::gravity;

const bool Actor::LEFT = false;
const bool Actor::RIGHT = true;

Actor::Actor() : should_remove(false)
{
}

Actor::~Actor()
{
}

void Actor::moveWithFPS(float offsetX, float offsetY)
{
	Sprite::move(offsetX * fps->asSeconds(), offsetY * fps->asSeconds());
}

void
Actor::setMap(int width, int height)
{
	map_width = width;
	map_height = height;
}

void
Actor::setTimeFPS(sf::Time* time)
{
	fps = time;
}

void
Actor::update()
{
}

void
Actor::look(bool direction)
{
	if (direction == LEFT)
	{
		setScale(-1, 1);
		this->direction = LEFT;
	} else {
		setScale(1, 1);
		this->direction = RIGHT;
	}
}
