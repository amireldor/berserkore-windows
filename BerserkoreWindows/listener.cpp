#include "listener.hpp"

using namespace bk;

Listener::Listener(int group)
	: not_interested(false)
{
	this->group = group;
}