#ifndef GAMELOOP_HPP
#define GAMELOOP_HPP

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "mainloopbase.hpp"
#include "commondata.hpp"
#include "actor.hpp"
#include "enemy.hpp"
#include "bomb.hpp"
#include "pubsub.hpp"
#include "hero.hpp"
#include "shot.hpp"
#include "cooldown.hpp"
#include "particle.hpp"
#include "soundstack.hpp"
#include "textinput.hpp"
#include "effecttext.hpp"
#include "happyhealth.hpp"

namespace bk
{

struct GameMainLoop : public MainLoopBase, public Listener,
	public boost::enable_shared_from_this<GameMainLoop>
{
	GameMainLoop(YAML::Node *n_config, sf::RenderWindow *n_window, PubSub *n_pubsub, ResourcePointer);
	virtual ~GameMainLoop();

	/**
	 * CommonGameData contains stuff, that we sometimes want to give Actors...
	 * e.g... the ResourceManager! But it's ok.
	 */
	CommonGameData data;

	void prepare();
	void processEvent(sf::Event);
	bool update();
	void draw();
	void cleanup();

	virtual void onNotify(const std::string &message, boost::any data);

	void newLevel();
	void newGrenade();
	void newBomb();
	void onGrenadeHitGround(Bomb*);
	void onNewShot(boost::shared_ptr<Shot>);
	void endGame();

	/**
	 * I'm too lazy to put this in the particle emitters/whatevers and
	 * schedule is getting tight.
	 */
	void createExplosion(unsigned int how_many, sf::Vector2f origin, YAML::Node explosion_conf);

	boost::shared_ptr<GroundView> ground_view;

	ActorList actors;
	ActorList shots;
	ActorList enemysoldiers;
	ActorList particles;
	boost::shared_ptr<HappyHealth> happyhealth; // to accompany Hero

	unsigned int map_width, map_height;
	unsigned int level; // how many enemies on this map ground

	// These are functions to run on an `ActorList`. I have many actors lists.
	// TODO: do I need this _coolest hack ever_ with the `on_each_actor`
	// function pointer (on update_erase_old_actors)?  or do I do
	// everything in the model layer inside the solider/shot/etc classes?
	void update_erase_old_actors(ActorList &my_actors, boost::function<void (Actor*)> on_each_actor);
	void draw_actors(ActorList &my_actors);
	void dummy() {}

	// helper "blood creation" function
	void bloodExplosion(sf::Vector2f position, unsigned int how_many=3);

	sf::RectangleShape red_rectangle;
	void calculateRedRectangleFill();

	void scoreChange(int change);

private:
	CoolDown bomb_cooldown, regenerate_cooldown;
	CoolDown gameover_cooldown; // used after dying to indicate when the Loop ends
	SoundStack shot_soundstack, bomb_soundstack;
	TextInput textinput;

	sf::Sound death_sound;

	int score;
	float score_factor; // read from config, adjusts the 'overall score' highness
	float visible_score; // the score currently being viewed, as we 'run' towards the real `score`
	float score_refresh_speed; // how many points we can 'move' per second in `visible_score`
	EffectText text_score;
	EffectText text_level;
	sf::Text text_tutorial;
	unsigned int tutorial_max_level;
	bool tutorial_show;

	void glowScore(const sf::Color&);

	// we change the view before printing text stuff
	sf::View view_for_text, orig_view;

	bool please_end_loop;
};

} // namespace bk

#endif
