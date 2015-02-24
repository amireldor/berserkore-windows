#ifndef LISTENER_HPP
#define LISTENER_HPP

#include <string>
#include <boost/any.hpp>
namespace bk
{

class Listener
{
public:
	virtual void onNotify(const std::string &message, boost::any data) = 0;
};

}
#endif
