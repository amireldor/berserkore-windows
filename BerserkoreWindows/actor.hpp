/**
 * actor.hpp
 *
 * Actor is a thing on the screen.  In Berserkore it will be the hero,
 * enemies, bombs, and grenades. And stuff.
 *
 * Check out the static setMap() and setTimeFPS() methods because you need to
 * set them up.
 *
 * Apparently, there's also Team here which is Actor related.
 */

#ifndef ACTOR_H
#define ACTOR_H

#include <list>
#include <SFML/Graphics.hpp>
#include <boost/shared_ptr.hpp>

namespace bk
{

class Actor;
typedef std::list< boost::shared_ptr<Actor> > ActorList;

namespace Team
{
	const int NEUTRAL = 0;
	const int GREEN = 1;
	const int RED = 2;
}

class Actor : public sf::Sprite
{
public:
	Actor();
	virtual ~Actor();

	/**
	 * The map dimensions used in all kinds of am-I-off-screen calculations
	 */
	static void setMap(int width, int height);

	/**
	 * The time used to calculate the FPS for movement
	 */
	static void setTimeFPS(sf::Time *time);

	static int getMapWidth() { return map_width; };
	static int getMapHeight() { return map_height; };

	/**
	 * Moves according to FPS
	 */
	void moveWithFPS(float offsetX, float offsetY);

	/**
	 * Use to read the FPS
	 */
	static const sf::Time &getFps() { return *fps; };

	/**
	 * Call in each frame
	 */
	virtual void update();

	/**
	 * Set the gravity or 'increment y' for all actors
	 */
	static void setGravity(float f) { gravity = f; }
	static float getGravity() { return gravity; }

	bool dont_keep() { return should_remove; }

	static const bool LEFT;
	static const bool RIGHT;

	void look(bool direction);
	bool getDirection() const { return direction; }
	float getSpeed() const { return speed; }
	float speed; // pixels per second for walking left/right
	/**
	 * look! I even made that virtual so you can put 'hooks' in it,
	 * e.g... unsubscribing from events if you are a Listener?
	 */
	virtual void shouldRemove(bool should=true) { should_remove = should; }

protected:
	bool should_remove; // Should you remove me from further iterations?
	bool direction;

private:
	static int map_width, map_height;
	static sf::Time* fps; // the FPS from the main loop
	static float gravity; // or more likely 'increment y'
};

}

#endif
