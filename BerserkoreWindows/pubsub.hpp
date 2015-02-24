#ifndef PUBSUB_H
#define PUBSUB_H

#include <string>
#include <list>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>

namespace bk
{

class Listener;
typedef std::list< boost::shared_ptr<Listener> > ListenersList;
typedef std::map< std::string, ListenersList > ListenersMap;

class PubSub
{
public:
	void publish(const std::string &message, boost::any data=0);
	void subscribe(const std::string &message, boost::shared_ptr<Listener> subscriber);
	void unsubscribe(const std::string &message, boost::shared_ptr<Listener> subscriber);
	void unsubscribe_from_all(boost::shared_ptr<Listener> subscriber);
private:
	// map of "message" to "listeners" to that message
	ListenersMap listeners_map;

};

}
#endif
