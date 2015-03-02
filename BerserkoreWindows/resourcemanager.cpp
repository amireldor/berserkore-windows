#include "resourcemanager.hpp"

using namespace bk;

void ResourceManager::setDataFolderWithSlash(const std::string &path)
{
	// could use some asserts or checks, no? TODO
	data_folder_slash = path;
}

boost::shared_ptr<sf::Texture> ResourceManager::getTexture(const std::string &what)
{
	return textureLoader.getResource(what);
}


boost::shared_ptr<sf::SoundBuffer> ResourceManager::getSoundBuffer(const std::string &what)
{
	return soundBufferLoader.getResource(what);
}

boost::shared_ptr<sf::Font> ResourceManager::getFont(const std::string &what)
{
	return fontLoader.getResource(what);
}

bool ResourceManager::loadTexture(const std::string &what)
{
	return textureLoader.loadResource(data_folder_slash, what);
}

bool ResourceManager::loadSoundBuffer(const std::string &what)
{
	return soundBufferLoader.loadResource(data_folder_slash, what);
}

bool ResourceManager::loadFont(const std::string &what)
{
	return fontLoader.loadResource(data_folder_slash, what);
}
