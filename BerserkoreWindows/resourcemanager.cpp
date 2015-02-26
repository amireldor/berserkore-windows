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
	boost::shared_ptr<sf::Texture> ret = textures[what];
	return ret;
}

boost::shared_ptr<sf::SoundBuffer> ResourceManager::getSoundBuffer(const std::string &what)
{
	boost::shared_ptr<sf::SoundBuffer> ret = sound_buffers[what];
	return ret;
}

boost::shared_ptr<sf::Font> ResourceManager::getFont(const std::string &what)
{
	boost::shared_ptr<sf::Font> ret = fonts[what];
	return ret;
}

bool ResourceManager::loadTexture(const std::string &what)
{
	if (!textures.count(what))
	{
		boost::shared_ptr<sf::Texture> item(new sf::Texture);
		if (item->loadFromFile(data_folder_slash + what))
		{
			textures[what] = item;
			return true;
		}
		return false; // failed loading
	}
	return true; // already loaded
}

bool ResourceManager::loadSoundBuffer(const std::string &what)
{
	if (!sound_buffers.count(what))
	{
		boost::shared_ptr<sf::SoundBuffer> item(new sf::SoundBuffer);
		if (item->loadFromFile(data_folder_slash + what))
		{
			sound_buffers[what] = item;
			return true;
		}
		return false; // failed loading
	}
	return true; // already loaded
}

bool ResourceManager::loadFont(const std::string &what)
{
	if (!fonts.count(what))
	{
		boost::shared_ptr<sf::Font> item(new sf::Font);
		if (item->loadFromFile(data_folder_slash + what))
		{
			fonts[what] = item;
			return true;
		}
		return false; // failed loading
	}
	return true; // already loaded
}
