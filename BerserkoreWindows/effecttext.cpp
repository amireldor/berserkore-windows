#include "effecttext.hpp"

using namespace bk;

void EffectText::update(float dt)
{
	std::vector<EffectBasePointer>::iterator iter = effects.begin();
	while (iter != effects.end())
	{
		(*iter)->update(dt);
		if ((*iter)->hasEnded())
		{
			iter = effects.erase(iter);
			continue;
		}
		iter++;
	}
}

void EffectText::addEffect(EffectBasePointer n_effect)
{
	effects.push_back(n_effect);
}

unsigned int EffectText::countEffects() const
{
	return effects.size();
}

EffectBase::EffectBase(EffectText *n_parent, float n_time)
{
	parent = n_parent;
	ended = false;
	time = n_time;
}

void EffectText::clearEffects()
{
	effects.clear();
}


ColorChange::ColorChange(EffectText *n_parent, float n_time, sf::Color n_end_color):
	EffectBase(n_parent, n_time),
	end_color(n_end_color)
{
	/**
	 * We'll keep thte original n_time in `time` and keep track of progress
	 * with `progress_time`.
	 */
	progress_time = n_time;
	start_color = n_parent->getColor();
}

void ColorChange::update(float dt)
{
	if (ended) return;

	progress_time -= dt;
	if (progress_time <= 0)
	{
		progress_time = 0;
		ended = true;
	}

	float progress =
		static_cast<float>(time-progress_time) / time; // progress from 0 .. 1

	sf::Uint8 r = (end_color.r - start_color.r) * progress + start_color.r;
	sf::Uint8 g = (end_color.g - start_color.g) * progress + start_color.g;
	sf::Uint8 b = (end_color.b - start_color.b) * progress + start_color.b;

	parent->setColor(sf::Color(r, g, b));
}
