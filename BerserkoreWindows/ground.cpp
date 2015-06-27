#include <ctime>
#include <cmath>
#include "ground.hpp"

#include "defs.hpp"

using namespace bk;

const float Ground::CONTINUE = -1.0f;

Ground::Ground(unsigned int size)
{
	this->data = boost::shared_array<float>(new float[size]);
	this->size = size;
	random_gen.seed(std::time(nullptr));
}

Ground::~Ground()
{
}

float Ground::createRandom(float start_height, float cursor_random_delta)
{
	if (start_height == CONTINUE) start_height = data[size-1];
	float &delta = cursor_random_delta;
	boost::random::uniform_real_distribution<float>
		distribution(-delta, +delta);

	// if `start_height` too low or too high, will try to balance out
	// things, see the cursor loop below
	float way_to_go = .5f - start_height;
	float balance_to_middle = way_to_go / this->size;

	// the actual interesting stuff
	float cursor = start_height;
	for (unsigned int i = 0; i < size; i++)
	{
		data[i] = cursor;
		cursor += distribution(random_gen) + balance_to_middle;
		if (cursor < 0.0f) cursor = 0.0f;
		if (cursor > 1.0f) cursor = 1.0f;
	}

	return cursor;
}

float Ground::operator[] (unsigned int index)
{
	if (index >= size) throw GroundException("Index out of bounds");
	return data[index];
}

void
Ground::crater(unsigned int x, float width, float depth)
{
	float alpha = 0, alpha_inc = M_PI / width; // for trigo fun
	for (int cursor = x-width/2; cursor<x+width/2; cursor++)
	{
		alpha += alpha_inc;
		if (cursor < 0) continue;
		if (cursor >= static_cast<int>(size)) break;
		float addition = depth * sinf(alpha);
		if (addition < 0) addition = 0;
		data[cursor] += addition;
		if (data[cursor] > 1.0f) data[cursor] = 1.0f;
	}
}

GroundView::GroundView(Ground* ptr,
	unsigned int width, unsigned int height)
{
	ground_ptr = ptr;
	this->width = width;
	this->height = height;

	// TODO: consider lazy-init
	texture.create(width, height);
	sprite.setTexture(texture);
	random_gen.seed(std::time(nullptr));
}

GroundView::~GroundView()
{
}

sf::Color GroundView::randomColor()
{
	// TODO: put in config.yaml
	sf::Uint8 deviation = 20;
	boost::random::uniform_int_distribution<sf::Uint8> dist(0, deviation*2);
	return sf::Color(
		130 + dist(random_gen),
		44 + dist(random_gen),
		4 + dist(random_gen)
	);
}

void GroundView::updateTexture()
{
	typedef sf::Uint8* PixelArray;
	PixelArray pixels(new sf::Uint8[4 * width * height]);
	memset(pixels, 0, 4 * width * height);

	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			unsigned int ground_y = static_cast<unsigned int>((*ground_ptr)[x] * height);
			if (y < ground_y) continue;

			sf::Color color = randomColor();

			pixels[y*4*width + x*4 + 0] = color.r;
			pixels[y*4*width + x*4 + 1] = color.g;
			pixels[y*4*width + x*4 + 2] = color.b;
			pixels[y*4*width + x*4 + 3] = 255;
		}
	}

	texture.update(pixels);
}
