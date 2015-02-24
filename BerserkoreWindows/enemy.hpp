#ifndef ENEMY_H
#define ENEMY_H

#include <yaml-cpp/yaml.h>
#include <boost/enable_shared_from_this.hpp>
#include "soldier.hpp"
#include "commondata.hpp"
#include "listener.hpp"

namespace bk
{

class EnemySoldier : public Soldier, public Listener,
	public boost::enable_shared_from_this<EnemySoldier>
{
public:
	EnemySoldier(YAML::Node *config, CommonGameData data);
	virtual ~EnemySoldier();
	void update();

	virtual void onNotify(const std::string &message, boost::any data);
	void shouldRemove(bool should=true);
	void putOnGround();

protected:
	CoolDown fire_start_stop_cooldown;
	bool shooting;
};

}

#endif
