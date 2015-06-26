#include "pubsub.hpp"
#include "listener.hpp"

using namespace bk;

void PubSub::subscribe(const std::string &message, boost::shared_ptr<Listener> subscriber, int group)
{
	ListenersList &the_list = listeners_map[message];
	subscriber->group = group;
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
		++iter;
	}
}

void PubSub::publish(const std::string &message, boost::any data)
{
	ListenersList listeners = listeners_map[message];
	ListenersList::iterator iter = listeners.begin();
	while (iter != listeners.end())
	{
		(*iter)->onNotify(message, data);
		++iter;
	}
}

#include <iostream> // remove me
void PubSub::clear_group(int group)
{
	for (ListenersMap::iterator iter = listeners_map.begin(); iter != listeners_map.end(); ++iter) {
		ListenersList listeners_list = iter->second;
		//listeners_list.clear();
		//iter->second.clear();
		for (ListenersList::iterator listener = listeners_list.begin(); listener != listeners_list.end(); ) {
			//listener = listeners_list.erase(listener);
			listener = ++listener;
			listeners_list.pop_front();
		}
		/*
		ListenersList listeners_list = iter->second;
		for (ListenersList::iterator listener = listeners_list.begin(); listener != listeners_list.end(); ) {
			if ((*listener)->group == group) {
				listener = listeners_list.erase(listener);
				continue;
			}
			++listener;
		}*/
	}
}

void PubSub::clear_all() {
	listeners_map.clear();
}
