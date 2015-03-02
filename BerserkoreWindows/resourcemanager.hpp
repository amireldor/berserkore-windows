/**
 * resourcemanager.hpp
 * The place to load stuff like textures and then ask for them wherever you
 * need them.
 *
 * Note that I am smart* and it's not a singleton so there can be several
 * managers with different data folders and such. Am I smart?
 *
 * * NO! I did bad things! resources should be available to everyone! now
 * they are only available to the main loops! This is silly!
 *
 */
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <stdexcept>
#include <map>
#include <string>
#include <yaml-cpp/yaml.h>
#include <boost/shared_ptr.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace bk
{

// template for SFML resource loader (uses their ->loadFromFile
template <class Resource>
class ResourceLoader
{
public:
	ResourceLoader();

	boost::shared_ptr<Resource> getResource(const std::string &what);
	bool loadResource(const std::string &data_dir, const std::string &what);

private:
	std::map< std::string, boost::shared_ptr<Resource> > data;
};

template <class Resource>
boost::shared_ptr<Resource> ResourceLoader<Resource>::getResource(const std::string &what)
{
	return data[what];
}

template <class Resource>
ResourceLoader<Resource>::ResourceLoader()
{
}

template <class Resource>
bool ResourceLoader<Resource>::loadResource(const std::string &data_dir, const std::string &what)
{
	// make sure data_dir ends with '/'
	std::string correct_data_dir = data_dir;
	if (correct_data_dir.back() != '/') correct_data_dir = data_dir + "/";

	if (!data.count(what))
	{
		boost::shared_ptr<Resource> item(new Resource);
		if (item->loadFromFile(correct_data_dir + what))
		{
			data[what] = item;
			return true;
		}
		return false; // failed loading
	}
	return true; // already loaded
}

// contains all resource loaders
class ResourceManager
{
public:
	void setDataFolderWithSlash(const std::string &path);

	bool loadTexture(const std::string &what);
	bool loadSoundBuffer(const std::string &what);
	bool loadFont(const std::string &what);

	boost::shared_ptr<sf::Texture> getTexture(const std::string &what);
	boost::shared_ptr<sf::SoundBuffer> getSoundBuffer(const std::string &what);
	boost::shared_ptr<sf::Font> getFont(const std::string &what);

private:
	ResourceLoader<sf::Texture> textureLoader;
	ResourceLoader<sf::SoundBuffer> soundBufferLoader;
	ResourceLoader<sf::Font> fontLoader;

	std::string data_folder_slash;

};

typedef boost::shared_ptr<ResourceManager> ResourcePointer;

//class ResourceException : public std::runtime_error {};

}

#endif
