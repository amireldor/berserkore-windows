/**
 *
 * OH MY GOD! SO MUCH DUPLICATION OF CODE HERE
 * See all of these get* functions... I can think of a template@!12%
 * But then again... dunno... it's not the time to think right now.
 *
 */
#include "resourcemanager.hpp"

using namespace bk;

void ResourceManager::setDataFolderWithSlash(const std::string &path)
{
	// could use some asserts or checks, no? TODO
	data_folder_slash = path;
}

boost::shared_ptr<sf::Texture> ResourceManager::getTexture(const std::string &what)
{
	// FIXME, [] won't return NULL ever, use .count
	boost::shared_ptr<sf::Texture> ret = textures[what];
	if (ret == NULL)
	{
		ret = boost::shared_ptr<sf::Texture>(new sf::Texture);
		// no texture loaded with this name, let's load it
		// FIXME No error checking!
		ret->loadFromFile(data_folder_slash + what);
		textures[what] = ret;
	}
	return ret;
}

boost::shared_ptr<sf::SoundBuffer> ResourceManager::getSoundBuffer(const std::string &what)
{
	// FIXME, [] won't return NULL ever, use .count
	boost::shared_ptr<sf::SoundBuffer> ret = sound_buffers[what];
	if (ret == NULL)
	{
		ret = boost::shared_ptr<sf::SoundBuffer>(new sf::SoundBuffer);
		// FIXME No error checking!
		ret->loadFromFile(data_folder_slash + what);
		sound_buffers[what] = ret;
	}
	return ret;
}

boost::shared_ptr<sf::Font> ResourceManager::getFont(const std::string &what)
{
	if (fonts.count(what))
	{
		// font is found
		return fonts[what];
	}

	// no font, create new
	boost::shared_ptr<sf::Font> new_font(new sf::Font);
	new_font->loadFromFile(data_folder_slash + what);
	fonts[what] = new_font;
	return new_font;

	//^^^ still not working as expected :-(

/*	boost::shared_ptr<sf::Font> ret = fonts[what];
	if (ret == NULL)
	{
		ret = boost::shared_ptr<sf::Font>(new sf::Font);
		// FIXME No error checking!
		ret->loadFromFile(data_folder_slash+ what);
		fonts[what] = ret;
	}
	return ret;*/
}
