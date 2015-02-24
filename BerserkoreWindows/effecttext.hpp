#ifndef	EFFECTTEXT_HPP
#define EFFECTTEXT_HPP

#include <boost/shared_ptr.hpp>
#include <SFML/Graphics.hpp>
#include "actor.hpp"

namespace bk
{

class EffectBase;
typedef boost::shared_ptr<EffectBase> EffectBasePointer;

class EffectText : public sf::Text
{
public:
	void update(float dt);
	void addEffect(EffectBasePointer n_effect);
	void clearEffects();

protected:
	std::vector<EffectBasePointer> effects;
};

//----------------------------------------
class EffectBase
{
public:
	EffectBase(EffectText *n_parent, float n_time=5);
	virtual void update(float dt) = 0;
	void setTime(float n_time) { time = n_time; }
	bool hasEnded() { return ended; }

protected:
	EffectText *parent;
	float time;
	bool ended;
};

//----------------------------------------
/**
 * Will take n_parent's color and interpolate it to n_end_coor over n_time
 * seconds.
 */
class ColorChange : public EffectBase
{
public:
	ColorChange(EffectText *n_parent, float n_time, sf::Color n_end_color);
	void update(float dt);

protected:
	float progress_time;
	sf::Color end_color;
	sf::Color start_color;
};

} // namespace lm

#endif
