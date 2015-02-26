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

class ResourceManager
{
public:
	void setDataFolderWithSlash(const std::string &path);

	/**
		These won't load the texture in the lazy-load method used previousy.
		Use loadXXXX functions.
	*/
	boost::shared_ptr<sf::Texture> getTexture(const std::string &what);
	boost::shared_ptr<sf::SoundBuffer> getSoundBuffer(const std::string &what);
	boost::shared_ptr<sf::Font> getFont(const std::string &what);

	bool loadTexture(const std::string &what);
	bool loadSoundBuffer(const std::string &what);
	bool loadFont(const std::string &what);

private:
	std::map< std::string, boost::shared_ptr<sf::Texture> > textures;
	std::map< std::string, boost::shared_ptr<sf::SoundBuffer> > sound_buffers;
	std::map< std::string, boost::shared_ptr<sf::Font> > fonts;
	std::string data_folder_slash;

};

typedef boost::shared_ptr<ResourceManager> ResourcePointer;

//class ResourceException : public std::runtime_error {};

}

#endif
