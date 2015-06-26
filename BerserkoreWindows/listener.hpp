#ifndef LISTENER_HPP
#define LISTENER_HPP

#include <string>
#include <boost/any.hpp>
namespace bk
{

class Listener
{
public:
	Listener(int group = 0);

	virtual void onNotify(const std::string &message, boost::any data) = 0;
	int group;
	bool not_interested; // I don't want to listen anymore! hell, remove me from the list!
};

}
#endif
