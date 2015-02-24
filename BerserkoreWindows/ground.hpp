/**
 * Ground data is 0.0f to 1.0f where 0 is top and 1 is bottom.
 * This file also contains GroundView for showing ground stuff.
 */
#ifndef _GROUND_H_
#define _GROUND_H_

#include <cstring> // for memset
#include <stdexcept>
#include <boost/shared_array.hpp>
#include <boost/scoped_array.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <SFML/Graphics.hpp>

namespace bk
{

class Ground
{
public:

	Ground(unsigned int size);
	virtual ~Ground();

	/**
	 * Create ground randomly.
	 * @return The final height of the ground created.
	 * Give start_height of -1 for 'continuing' from rightmost ground
	 * (i.e. false scrolling)
	 */
	static const float CONTINUE;
	float createRandom(float start_height=.5f, float cursor_random_delta=.007f);
	float operator[] (unsigned int index);

	/**
	 * NOTE! `depth` here is in the same scale as the values: 0..1.0
	 * small stuff! the ground data is 0.0-1.0f
	 */
	void crater(unsigned int x, float width, float depth);

protected:

private:
	boost::shared_array<float> data;
	unsigned int size;

	boost::random::mt19937 random_gen; // for height randomization
};

struct GroundException : public std::runtime_error {
	GroundException(const std::string& msg) : std::runtime_error(msg) {}
};

class GroundView
{
public:
	/**
	 * @param width should be the same as the ground_ptr's width
	 */
	GroundView(Ground*, unsigned int width, unsigned int height);
	virtual ~GroundView();

	/**
	 * Sync between the data in ground_ptr and the texture we store
	 */
	void updateTexture();

	/**
	 * Get a nice random ground-brown color
	 */
	sf::Color randomColor();

	sf::Sprite& getSprite() { return sprite; }

protected:

private:
	Ground* ground_ptr;
	unsigned int width, height;

	sf::Sprite sprite;
	sf::Texture texture;
	boost::random::mt19937 random_gen; // for colors
};

}

#endif
