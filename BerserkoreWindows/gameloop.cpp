#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include "gameloop.hpp"
#include "go.hpp"
#include "particleemitter.hpp"
#include "loopfactory.hpp"

using namespace bk;

void massShouldRemove(const ActorList &);

GameMainLoop::GameMainLoop(YAML::Node *n_config, sf::RenderWindow *n_window, PubSub *n_pubsub, ResourcePointer n_resource):
	MainLoopBase(n_config, n_window, n_pubsub, n_resource),
	shot_soundstack(4), bomb_soundstack(2)
{
}

#include <iostream> // DEBUG
GameMainLoop::~GameMainLoop()
{
std::cout << "cat bye" << std::endl;
}

void GameMainLoop::prepare()
{
	data = new CommonGameData();
	data->game = shared_from_this();
	MainLoopBase::prepare();

	map_width = (*config)["map"][0].as<unsigned int>();
	map_height = (*config)["map"][1].as<unsigned int>();

	// Create stuff and window
	data->ground = boost::shared_ptr<Ground>(new Ground(map_width));
	data->ground->createRandom();

	ground_view = boost::shared_ptr<GroundView>(new GroundView(data->ground.get(), map_width, map_height));
	ground_view->updateTexture();

	data->pubsub = MainLoopBase::pubsub;
	data->pubsub->subscribe("map:new", shared_from_this());
	data->pubsub->subscribe("grenade:new", shared_from_this());
	data->pubsub->subscribe("grenade:hits_ground", shared_from_this());
	data->pubsub->subscribe("shot:new", shared_from_this());
	data->pubsub->subscribe("enemy:tango_down", shared_from_this());
	data->pubsub->subscribe("hero:got_shot", shared_from_this());
	//data->pubsub->subscribe("hero:dead", shared_from_this());

	please_end_loop = false;

	data->resources = resources; // hack hack YEAH BABY! see explanation:
	/**
	 * As said in the .hpp file, we sometime need `data` in Actors or wherever dunno,
	 * but resources is a very nice object! So the main loop has it.
	 * You see... the welcome screen might want to use some resources.
	 */

	Actor::setMap(map_width, map_height);
	Actor::setTimeFPS(&frameTime);
	Actor::setGravity((*config)["gravity"].as<float>());

	// Hero
	data->hero = boost::make_shared<Hero>(config, data, (*config)["hero"]["speed"].as<float>());

	data->hero->setPosition((map_width * 0.44f), 0); // the Y is calculated in main loop
	actors.push_back(data->hero);

	happyhealth = boost::shared_ptr<HappyHealth>(new HappyHealth(
		data->hero.get(),
		(*config)["happyhealth"]["rate"].as<float>()
	));
	happyhealth->setTexture( *(resources->getTexture((*config)["particles_texture"].as<std::string>())) );
	happyhealth->setTextureRect( main_texture_subrect_selector.rect(3, 0) );
	happyhealth->show(false);

	level = 0;

	bomb_cooldown.lock(); // don't be ready

	// text stuff
	orig_view = window->getView();
	view_for_text = sf::View(sf::FloatRect(
		0, 0,
		(*config)["text"]["view"][0].as<float>(),
		(*config)["text"]["view"][1].as<float>()
	));

	text_score.setFont(*(resources->getFont((*config)["main_font"].as<std::string>())));
	text_score.setString("Hello!");

	text_level.setFont(*(resources->getFont((*config)["main_font"].as<std::string>())));
	text_level.setCharacterSize(42);
	text_level.setString("Hi!");
	text_level.setPosition(10, 10); // FIXME: should be a `text_margin` in config.yml

	text_tutorial.setFont(*(resources->getFont((*config)["main_font"].as<std::string>())));
	text_tutorial.setCharacterSize(14);
	text_tutorial.setPosition(10, 64);
	tutorial_show = true;
	tutorial_max_level = (*config)["tutorial"].size();
	text_tutorial.setString((*config)["tutorial"][level].IsDefined() ? (*config)["tutorial"][level].as<std::string>() : "");

	// score stuff
	score = 0;
	score_factor = (*config)["game"]["score_factor"].as<float>();
	visible_score = (float)score;
	score_refresh_speed = 89.f * score_factor;


	// the Go banner
	boost::shared_ptr<Go> go(new Go(config));
	data->pubsub->subscribe("map:new", go);
	go->setTexture(*resources->getTexture((*config)["go"]["texture"].as<std::string>()));
	actors.push_back(go);

	// big red (transparent) rectangle
	red_rectangle.setSize(sf::Vector2f((float)map_width, (float)map_height));
	calculateRedRectangleFill();

	// regeneration stuff
	regenerate_cooldown.set_maximum((*config)["life"]["regenerate_delay"].as<float>());

	// game over stuff
	gameover_cooldown.set_maximum((*config)["life"]["gameover_wait"].as<float>());
	gameover_cooldown.lock();

	// death sound, i don't feel like lazy-loading it when dead
	death_sound = sf::Sound( *resources->getSoundBuffer((*config)["death"]["sound"].as<std::string>()) );
}

void GameMainLoop::calculateRedRectangleFill()
{
	float inv_life = 1.0f - data->hero->getLife();
	if (inv_life < 0.f) inv_life = 0.f;
	if (inv_life > 1.f) inv_life = 1.f;
	red_rectangle.setFillColor(sf::Color(255, 0, 0, (sf::Uint8)(inv_life*255.f)));
}

void GameMainLoop::processEvent(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		// hello. welcome to the ugliest if-else in the universe.
		// this is so silly and i don't know why i did that, but i'm zorem.
		if (event.key.code == sf::Keyboard::Return)
		{
			data->hero->try_throw_grenade();
                        // FIXME: WHY IS THIS HERE IT MAKES NO SENSE
		}
		else if (event.key.code == sf::Keyboard::Escape) 
		{
			please_end_loop = true;
		}
		else if (event.key.code == sf::Keyboard::H) {
			tutorial_show = !tutorial_show;
		}

	} else if (event.type == sf::Event::TextEntered)
	{
		textinput.processTextEntered(event);
		// TODO implement a handleTextInput() or something
		std::string the_text = textinput.getString();
		if (the_text.find("zingabo") != std::string::npos)
		{
			newLevel();
			textinput.clear();
		} else if (the_text.find("badbad") != std::string::npos) {
			level = 15;
			newLevel();
			textinput.clear();
		} else if (the_text.find("boo") != std::string::npos) {
			newBomb();
			textinput.clear();
		}

		if (the_text.size() > 100)
		{
			textinput.shiftleft();
		}
	}
}

bool GameMainLoop::update()
{
	/* TODO:
	 * I use the `dummy` function too much throughout here
	 */

	// Update enemy stuff
	//update_erase_old_actors(enemysoldiers, boost::bind(&GameMainLoop::enemy_soldier_update, this));
	update_erase_old_actors(enemysoldiers,
		boost::bind(&GameMainLoop::dummy, this));

	// Update shots
	// checks collision with enemy soldiers and sets erase flags
	update_erase_old_actors(shots, boost::bind(&GameMainLoop::dummy, this));

	// Update rest
	update_erase_old_actors(actors, boost::bind(&GameMainLoop::dummy, this));

	// Maybe create a bomb
	if (level)
	{
		if (bomb_cooldown.update(frameTime.asSeconds()))
		{
			// MAYBE create a bomb, and reset cooldown
			if ((*config)["bombs"]["bomb"]["chance"].as<float>() >= rand_zero_to_one())
			{
				newBomb();
			}
			bomb_cooldown.lock();
		}
	}

	// Update particles
	update_erase_old_actors(particles, boost::bind(&GameMainLoop::dummy, this));

	// life and red rectangle stuff
	happyhealth->update(frameTime.asSeconds());

	if (!data->hero->isDead())
	{
		regenerate_cooldown.update(frameTime.asSeconds());
	}

	if (data->hero->getLife() < 1.0f && regenerate_cooldown.is_ready())
	{
		data->hero->addLife((*config)["life"]["regenerate"].as<float>() * frameTime.asSeconds());
		// happy indicator above hero's head
		happyhealth->show(true);
	} else {
		happyhealth->show(false);
	}
	calculateRedRectangleFill();

	// Update texts and score
	if (visible_score < score)
	{
		// score running up
		visible_score += score_refresh_speed * frameTime.asSeconds();
		if (visible_score > score)
		{
			visible_score = (float)score;
		}
	} else if (visible_score > score) { // note that there's no if-case for == as it's not needed
		// score running down
		visible_score -= score_refresh_speed * frameTime.asSeconds();
		if (visible_score < score) {
			visible_score = (float)score;
		}
	}


	std::stringstream ss;
	ss << floor(visible_score);

	text_score.update(frameTime.asSeconds());
	text_score.setString(ss.str());
	sf::FloatRect score_bounds = text_score.getLocalBounds();
	text_score.setOrigin(score_bounds.width, 0);
	text_score.setPosition( (view_for_text.getSize().x-10), 10 );

	text_level.update(frameTime.asSeconds());

	// determine if we should end the Loop
	if (data->hero->isDead())
	{
		if (gameover_cooldown.update(frameTime.asSeconds()))
		{
			please_end_loop = true;
		}
	}

	if (please_end_loop) {
		cleanup();
		return false;
	}

	return true;
}

void GameMainLoop::draw()
{
	sf::Color sky(0, 20, 90); // TODO: move to config.yaml
	window->clear(sky);

	window->draw(ground_view->getSprite());

	draw_actors(enemysoldiers);
	draw_actors(actors);
	window->draw(*happyhealth);
	draw_actors(shots);
	draw_actors(particles);

	// text stuff
	window->setView(view_for_text);
	window->draw(text_level);
	window->draw(text_score);
	if (tutorial_show &&  level < tutorial_max_level) window->draw(text_tutorial);
	window->setView(orig_view);

	// red rectangle
	if (!data->hero->isDead())
	{
		window->draw(red_rectangle);
	}

}

void GameMainLoop::onNotify(const std::string &message, boost::any data)
{
	if (message == "map:new")
	{
		newLevel();
	}
	else if (message == "grenade:new")
	{
		newGrenade();
		scoreChange(-34);
	}
	else if (message == "grenade:hits_ground")
	{
		Bomb *bomb = boost::any_cast<Bomb*>(data);
		onGrenadeHitGround(bomb);
	}
	else if (message == "shot:new")
	{
		boost::shared_ptr<Shot> shot = boost::any_cast< boost::shared_ptr<Shot> >(data);
		onNewShot(shot);
/*		if (shot->team == Team::GREEN)
		{
			scoreChange(12);
		}*/
	}
	else if (message == "enemy:tango_down")
	{
		sf::Vector2f death_position = boost::any_cast<sf::Vector2f>(data);
		bloodExplosion(death_position);
		scoreChange(155);
	}
	else if (message == "hero:got_shot")
	{
		regenerate_cooldown.lock();
		this->data->hero->reduceLife( (*config)["life"]["got_shot"].as<float>() );
		scoreChange(-57);
		if (this->data->hero->isDead())
		{
			endGame();
		} else {
			bloodExplosion(this->data->hero->getPosition());
		}
	}
}

void GameMainLoop::bloodExplosion(sf::Vector2f position, unsigned int how_many)
{
	createExplosion(how_many, position, (*config)["explosion"]["blood"]);
}

void GameMainLoop::newLevel()
{
	data->ground->createRandom(Ground::CONTINUE);
	// YOU! DON'T CLEAR THE `actors` YOU! THEY HAVE THE HERO INSTANCE ITSELF!
	massShouldRemove(shots);
	massShouldRemove(particles);
	shots.clear();
	particles.clear();
	level++;

	// do magic effects with text_level
	std::stringstream ss;
	ss << "Level: " << level;
	text_level.setString(ss.str());

	text_level.clearEffects();
	sf::Color orig_color = text_level.getColor();
	text_level.setColor(sf::Color(92, 255, 64)); // FIXME: be in config.yml someday
	EffectBasePointer the_effect(new ColorChange(&text_level, 1.2f, orig_color));
	text_level.addEffect(the_effect);

	// do magic effects with text_tutorial
	if (level < tutorial_max_level) {
		text_tutorial.setString((*config)["tutorial"][level].IsDefined() ? (*config)["tutorial"][level].as<std::string>() : "");
	}

	// this will gradually be quicker according to level
	bomb_cooldown.set_maximum(
		(*config)["bombs"]["bomb"]["base_every"].as<float>() / level
	);

	// create per-level stuff
	boost::random::uniform_int_distribution<int> dist_int(0, map_width-1);
	boost::random::uniform_real_distribution<float> dist_real(0, map_width-1.0f);
	massShouldRemove(enemysoldiers);
	enemysoldiers.clear();

	// 	craters on ground
	for (unsigned int i = 0; i < level; i++)
	{
		data->ground->crater(dist_int(random_gen), 44, 22.3f/240); // FIXME: hardcoded stuff :(
	}

	// 	enemies
	for (unsigned int i = 0; i < level; i++)
	{
		float x = dist_real(random_gen);

		boost::shared_ptr<EnemySoldier> newguy(new EnemySoldier(config, data));
		newguy->setPosition(x, 0);
		newguy->putOnGround();
		newguy->setTexture(*(resources->getTexture((*config)["main_texture"].as<std::string>())));
		newguy->setTextureRect(main_texture_subrect_selector.rect(0, 2));
		newguy->setOrigin(main_texture_subrect_selector.frame.x / 2.f,
				main_texture_subrect_selector.frame.y / 2.f);
		newguy->look(newguy->LEFT);
		data->pubsub->subscribe("grenade:hits_ground", newguy);

		enemysoldiers.push_back(newguy);
	}

	// prepare ground view after all craters were created
	ground_view->updateTexture();
}

void GameMainLoop::newGrenade()
{
	// create a new grenade at hero
	float ini_ix, ini_iy;
	float x_fac = 1; // for negating all ix if facing LEFT
	if (data->hero->getDirection() == Actor::LEFT)
	{
		x_fac = -1;
	}

	ini_iy = (*config)["bombs"]["grenade"]["ini_iy"].as<float>();

	if (data->hero->is_walking())
	{
		ini_ix = (*config)["bombs"]["grenade"]["ini_ix_in_movement"].as<float>();
	} else 	{
		ini_ix = (*config)["bombs"]["grenade"]["ini_ix"].as<float>();
	}
	ini_ix *= x_fac;

	boost::random::uniform_real_distribution<float> for_inc_rot(
		-(*config)["bombs"]["grenade"]["inc_rot_max"].as<float>(),
		(*config)["bombs"]["grenade"]["inc_rot_max"].as<float>()
	);
	float inc_rot = for_inc_rot(random_gen);

	boost::shared_ptr<bk::Bomb> grenade(
		new bk::Bomb(
			config,
			data,
			(*config)["bombs"]["grenade"]["width"].as<float>(),
			(*config)["bombs"]["grenade"]["depth"].as<float>(),
			ini_ix, ini_iy, inc_rot,
			bk::Bomb::textureGrenade,
			bk::Bomb::soundNameGrenade
		)
	);

	sf::Vector2f pos = data->hero->getPosition();
	pos.x += x_fac*(*config)["bombs"]["grenade"]["throw_position"][0].as<float>();
	pos.y += (*config)["bombs"]["grenade"]["throw_position"][1].as<float>();
	grenade->setPosition(pos);

	boost::random::uniform_real_distribution<float> rotation(0.0f, 360.0f);
	grenade->setRotation(rotation(random_gen));

	data->pubsub->subscribe("map:new", grenade);

	actors.push_back(grenade);
}

void GameMainLoop::newBomb()
{
	// create a new bomb at sky
	float ini_ix, ini_iy;
	boost::random::uniform_real_distribution<float> ini_ix_dist(
		-(*config)["bombs"]["bomb"]["ini_ix"].as<float>(),
		(*config)["bombs"]["bomb"]["ini_ix"].as<float>()
	);

	ini_ix = ini_ix_dist(random_gen);
	ini_iy = (*config)["bombs"]["bomb"]["ini_iy"].as<float>();

	boost::shared_ptr<bk::Bomb> bomb(
		new bk::Bomb(
			config,
			data,
			(*config)["bombs"]["bomb"]["width"].as<float>(),
			(*config)["bombs"]["bomb"]["depth"].as<float>(),
			ini_ix, ini_iy, 0,
			bk::Bomb::textureBomb,
			bk::Bomb::soundNameBomb
		)
	);


	boost::random::uniform_real_distribution<float> ini_rot_dest(
		(*config)["bombs"]["bomb"]["ini_rot"][0].as<float>(),
		(*config)["bombs"]["bomb"]["ini_rot"][1].as<float>()
	);
	float ini_rot = ini_rot_dest(random_gen);
	bomb->setRotation(ini_rot);

	boost::random::uniform_real_distribution<float> x_dist(0.0f, static_cast<float>(map_width));
	sf::Vector2f pos(x_dist(random_gen), (*config)["bombs"]["bomb"]["ini_y"].as<float>());
	bomb->setPosition(pos);

	data->pubsub->subscribe("map:new", bomb);

	// whistle!
	bomb_soundstack.play( *(resources->getSoundBuffer( (*config)["sounds"]["whistle"].as<std::string>())) );

	// add to actors list
	actors.push_back(bomb);
}

void GameMainLoop::onGrenadeHitGround(bk::Bomb *bomb)
{
	// Make crater
	sf::Vector2f pos = bomb->getPosition();
	float width = bomb->getWidth();
	float depth = bomb->getDepth() / map_height;

	data->ground->crater((unsigned int)pos.x, width, depth);
	ground_view->updateTexture();

	// reduce score a bit :( if too near to hero
	if (!data->hero->isDead())
	{
		float dist_from_hero = fabs(data->hero->getPosition().x - pos.x);
		if (dist_from_hero < width/2.f)
		{
			scoreChange( -(int)((width/2.f - dist_from_hero)*4.2f) ); // FIXME: 4.2 should be in config. need to stop being lazy
		}
	}

	// boom!
	bomb_soundstack.play( *(resources->getSoundBuffer( (*config)["sounds"][bomb->getSoundName()].as<std::string>())) );
	// TODO Create fire!

	// Some dirt flying around!
	createExplosion((unsigned int)width, pos, (*config)["explosion"]["dirt"]);
}

void GameMainLoop::onNewShot(boost::shared_ptr<Shot> shot)
{
	// Setup the new shot and let it fly
	//
	// texture
	shot->setTexture(*(resources->getTexture((*config)["main_texture"].as<std::string>())));
	shot->setTextureRect(main_texture_subrect_selector.rect(2, 3)); // TODO: this is ugly hardcoded
	// position
	shot->setOrigin(main_texture_subrect_selector.frame.x / 2.f, main_texture_subrect_selector.frame.y / 2.f);
	shot->advance((*config)["soldier"]["shot_start_pos_offset"].as<float>()); // move away from the center of the shooter
	// fly!
	shots.push_back(shot);
	// boom!
	shot_soundstack.play(
		*(resources->getSoundBuffer( (*config)["sounds"]["shot"].as<std::string>())),
		rand_zero_to_one()*0.1f + 1 // FIXME this is ugly hardcoded

	);
}

void GameMainLoop::update_erase_old_actors(ActorList &my_actors, boost::function<void (Actor*)> on_each_actor)
{
	ActorList::iterator iter;
	Actor *actor;

	iter = my_actors.begin();
	while (iter != my_actors.end())
	{
		actor = (*iter).get();
		on_each_actor(actor);
		actor->update();

		if (actor->dont_keep())
		{
			iter = my_actors.erase(iter);
			continue;
		}

		iter++;
	}
}

void GameMainLoop::draw_actors(ActorList &my_actors)
{
	ActorList::iterator iter;
	Actor *actor;

	iter = my_actors.begin();
	while (iter != my_actors.end()) {
		actor = (*iter).get();
		window->draw(*actor);
		iter++;
	}
}

void GameMainLoop::createExplosion(unsigned int how_many, sf::Vector2f origin, YAML::Node explosion_conf)
{
	const YAML::Node &conf = explosion_conf;
	const unsigned int subrect_x = conf["subrect"][0].as<unsigned int>();
	const unsigned int subrect_y = conf["subrect"][1].as<unsigned int>();

	boost::random::uniform_real_distribution<float>
		ix_dist(conf["ix"]["min"].as<float>(), conf["ix"]["max"].as<float>());
	boost::random::uniform_real_distribution<float>
		iy_dist(conf["iy"]["min"].as<float>(), conf["iy"]["max"].as<float>());
	boost::random::uniform_real_distribution<float>
		random_deg(0.0f, 360.0f);
	while (how_many--)
	{
		boost::shared_ptr<Particle> particle (new Particle);
		particle->setTexture(*resources->getTexture((*this->config)["particles_texture"].as<std::string>()));
		particle->setTextureRect(main_texture_subrect_selector.rect(subrect_x, subrect_y));
		particle->setPosition(origin);
		particle->ix = ix_dist(this->random_gen);
		particle->iy = iy_dist(this->random_gen);
		particle->iiy = particle->getGravity();
		particle->ttl = conf["ttl"].as<float>();
		particle->rotate(random_deg(this->random_gen));
		particles.push_back(particle);
	}
}

void GameMainLoop::endGame()
{
	sf::Vector2f hero_pos = data->hero->getPosition();
	actors.remove(data->hero);

	// emit a lot of blood
	boost::shared_ptr<ParticleEmitter> emitter(new ParticleEmitter(config, data, &particles, ParticleEmitter::preset("blood")));
	emitter->emitter_rate = 174.f;
	emitter->emitter_life = 4.62f;
	emitter->setPosition(hero_pos);
	emitter->move(0, -(main_texture_subrect_selector.frame.y*0.3f)); // adjust blood starting position
	actors.push_back(emitter);

	death_sound.play();

	next_loop = boost::shared_ptr<MainLoopBase>(LoopFactory::create(LoopFactory::DEATH, config, window, pubsub, resources));

}

void GameMainLoop::glowScore(const sf::Color& glow_color)
{
	text_score.clearEffects();
	sf::Color orig_color = sf::Color::White;
	text_score.setColor(glow_color);

	EffectBasePointer the_effect(new ColorChange(&text_score, 1.3f, orig_color));
	text_score.addEffect(the_effect);
}

void GameMainLoop::scoreChange(int change)
{
	if (change == 0) return;
	score += change * static_cast<int>(score_factor);

	// glow beautifully!
	if (change > 0)
	{
		glowScore(sf::Color::Green);
	} else {
		glowScore(sf::Color::Red);
	}
}

void GameMainLoop::cleanup()
{
	massShouldRemove(actors);
	massShouldRemove(shots);
	massShouldRemove(enemysoldiers);
	massShouldRemove(particles);

	actors.clear();
	shots.clear();
	enemysoldiers.clear();
	particles.clear();

	pubsub->unsubscribe_from_all(shared_from_this());
	//data->hero = nullptr; // why do I comment this? why does this break when hero is dead? ahh fuck it! random programming is the best!
	//data->game = nullptr;
	//data->ground = nullptr; // probably not needed, destroyed with game.
	//data->pubsub = nullptr; // it's global anyway, so why null it? it confuses the bombs that should unsubscribe themselves.
	//data->resources = nullptr; // also global. let's keep it.
	delete data;
}

void massShouldRemove(const ActorList &actor_list)
{
	BOOST_FOREACH( boost::shared_ptr<Actor> actor, actor_list )
	{
		actor->shouldRemove();
	}
}