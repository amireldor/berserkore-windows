#include <iostream>
#include <fstream>
using namespace std;
#include <cstdlib>
#include <queue>

#include <SFML/Graphics.hpp>
#include <yaml-cpp/yaml.h>

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

#include "loopfactory.hpp"
#include "volumecontrol.hpp"
#include "volumebar.hpp"
#include "resourcemanager.hpp"
#include "particleemitter.hpp"
#include "pubsub.hpp"

int main(int argc, char *argv[])
{
	std::cout << "Berserkore!" << std::endl;
	std::cout << "Version: Frantik" << std::endl;

	std::string config_file_path;

	// command line parameters fun
	po::options_description desc("Provided options");
	desc.add_options()
		("help", "shows stuff like this")
		("config", po::value<std::string>(&config_file_path)->default_value("data/config.yml"), "select a specific config file")
	;
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help"))
	{
		std::cout << desc << std::endl;
		return EXIT_SUCCESS;
	}

	YAML::Node config;
	// Load config
	try {
		std::cout << "Loading config file: " << config_file_path << std::endl;
		config = YAML::LoadFile(config_file_path); // TODO: catch exception if file does not exist
	} catch (YAML::BadFile e)
	{
		std::cout << "Couln't find configuration file :(" << std::endl;
		exit(EXIT_FAILURE);
	} catch (YAML::ParserException e)
	{
		std::cout << "Bad configuration" << std::endl;
		std::cout << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	// create the window
	const bool fullscreen = (config["fullscreen"].as<bool>() == true);
	sf::Uint32 window_style = sf::Style::Titlebar | sf::Style::Close;
	if (fullscreen)
	{
		window_style |= sf::Style::Fullscreen;
	}

	sf::RenderWindow window(
		sf::VideoMode(
			config["screen"][0].as<unsigned int>(),
			config["screen"][1].as<unsigned int>()
		),
		"Berserkore!",
		window_style
	);

	window.setMouseCursorVisible(false);

	// Set the icon
	sf::Image icon;
	icon.loadFromFile(config["data_folder"].as<std::string>() + config["images"]["icon"].as<std::string>());
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	// This view will scale to fit window
	window.setView(sf::View(sf::FloatRect(
		0, 0,
		config["map"][0].as<unsigned int>(), // map width
		config["map"][1].as<unsigned int>() // map height
	)));

	bk::LoopFactory loop_factory;
	bk::LoopFactory::LoopPointer current_loop;


	std::queue<bk::LoopFactory::LoopType> loop_queue;
	if (	!config["skip_welcome"].IsDefined() ||
		(config["skip_welcome"].IsDefined() && !config["skip_welcome"].as<bool>())
	   )
	{
		loop_queue.push(loop_factory.WELCOME);
	}
	loop_queue.push(loop_factory.GAME);
	loop_queue.push(loop_factory.WELCOME);

	bk::ParticleEmitter::initPresets(config);
	bk::PubSub main_pubsub;

	current_loop = loop_factory.create(loop_queue.front(), &config, &window, &main_pubsub);
	loop_queue.pop();
	current_loop->prepare();

	// volume stuff
	bk::VolumeControl volume;
	bk::VolumeBar volume_bar(sf::FloatRect(config["map"][0].as<float>()/2-42, 10, 84, 6));

	sf::Sound volume_beep;
	sf::SoundBuffer sound_beep_buffer;
	sound_beep_buffer.loadFromFile(config["data_folder"].as<std::string>() + config["sounds"]["beep"].as<std::string>());
	volume_beep.setBuffer(sound_beep_buffer);

	const YAML::Node yaml_mute = config["mute"];
	if (yaml_mute)
	{
		if (yaml_mute.as<bool>())
		{
			volume.muteToggle();
		}
	}

	// read volume from file, we save to this file later on (on clean exist [for now?])
	fstream vol_file("bk.volume", ios::in);
	if (vol_file.is_open())
	{
		float in_vol;
		vol_file >> in_vol;
		// range check, then set volume
		if (in_vol > 100.f) in_vol = 100.f;
		if (in_vol < 0) in_vol = 0.f;
		sf::Listener::setGlobalVolume(in_vol);
	}
	vol_file.close();

	// Main loop
	sf::Clock clock;
	bool window_active = true;
	window.setFramerateLimit(config["fps_limit"].as<unsigned int>());
	while (window.isOpen())
	{
		current_loop->frameTime = clock.restart();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			} else if (event.type == sf::Event::KeyPressed) {
				bool volume_changed = false;

				switch (event.key.code)
				{
				/*case sf::Keyboard::Escape:
					window.close();
					break;*/

				case sf::Keyboard::M:
				case sf::Keyboard::S:
					volume.muteToggle();
					volume_changed = true;
					break;

				case sf::Keyboard::Equal:
					volume.volUp();
					volume_changed = true;
					break;

				case sf::Keyboard::Dash:
					volume.volDown();
					volume_changed = true;
					break;
				default:
					break;
				}

				if (volume_changed)
				{
					volume_bar.appear();
					volume_beep.play();
				}
			} else if (event.type == sf::Event::LostFocus) {
				window_active = false;
			} else if (event.type == sf::Event::GainedFocus) {
				window_active = true;
			}

			current_loop->processEvent(event);

		}

		bool loop_continue;
		if (window_active)
		{
			loop_continue = current_loop->update();
		}
		if (!loop_continue)
		{
			if (loop_queue.empty())
			{
				window.close();
				continue;
			}

			current_loop = loop_factory.create(loop_queue.front(), &config, &window, &main_pubsub);
			loop_queue.pop();
			current_loop->prepare();
			continue;
		}

		current_loop->draw();

		volume_bar.value();
		volume_bar.update(); // Note that I don't heck for `window_active` here, dunno why.
		window.draw(volume_bar);

		window.display();
	}

	// write volume to file
	vol_file.open("bk.volume", ios::out | ios::trunc);
	if (vol_file.is_open())
	{
		vol_file << sf::Listener::getGlobalVolume();
	}

	std::cout << "And remember, YOU ARE THE MAGNIFICENT BERSERKORE. GOOD BYE." << std::endl;

	return EXIT_SUCCESS;
}
