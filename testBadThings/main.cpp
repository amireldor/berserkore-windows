#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace std;

class Thing;
class Hero;
class Game;

class GameStuff
{
public:
	boost::shared_ptr<Game> game;
	boost::shared_ptr<Thing> thing;
	boost::shared_ptr<Hero> hero;
};

class Thing
{
public:
	Thing() : friends(1) {}
	int friends;
};

class Hero
{
public:
	Hero(GameStuff*);
	void hero_magic();
	int age;
};

Hero::Hero(GameStuff *stuff)
{
	age = stuff->thing->friends;
}

void Hero::hero_magic()
{
	cout << "hero magic " << age << endl;
}

class Game : public boost::enable_shared_from_this<Game>
{
public:
	Game();
	void prepare();
	GameStuff *stuff;
	void game_magic();
};

Game::Game()
{
	stuff = new GameStuff();
}

void Game::prepare()
{
	stuff->game = shared_from_this();

	stuff->thing = boost::make_shared<Thing>();
	stuff->thing->friends = 12;

	stuff->hero = boost::make_shared<Hero>(stuff);
}

void Game::game_magic()
{
	cout << "game magic" << endl;
	cout << "-- calling hero:" << endl;
	stuff->hero->hero_magic();
	cout << "-- end" << endl;
}

int main(int argc, char *argv[])
{
	boost::shared_ptr<Game> game = boost::make_shared<Game>();
	game->prepare();
	game->game_magic();

	return 0;
}