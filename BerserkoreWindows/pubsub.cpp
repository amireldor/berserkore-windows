#include "pubsub.hpp"
#include "listener.hpp"

using namespace bk;

void PubSub::subscribe(const std::string &message, boost::shared_ptr<Listener> subscriber)
{
	ListenersList &the_list = listeners_map[message];
	the_list.push_back(subscriber);
}

void PubSub::unsubscribe(const std::string &message, boost::shared_ptr<Listener> subscriber)
{
	ListenersList &the_list = listeners_map[message];
	the_list.remove(subscriber);
}

void PubSub::unsubscribe_from_all(boost::shared_ptr<Listener> subscriber)
{
	ListenersMap::iterator iter = listeners_map.begin();

	while (iter != listeners_map.end())
	{
		iter->second.remove(subscriber);
		iter++;
	}
}

void PubSub::publish(const std::string &message, boost::any data)
{
	ListenersList listeners = listeners_map[message];
	ListenersList::iterator iter = listeners.begin();
	while (iter != listeners.end())
	{
		(*iter)->onNotify(message, data);
		iter++;
	}
}
