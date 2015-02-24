#include "textinput.hpp"
using namespace bk;

void TextInput::processEvent(const sf::Event &event)
{
	if (event.type == sf::Event::TextEntered)
	{
		processTextEntered(event);
	}
}

void TextInput::processTextEntered(const sf::Event &event)
{
	if (event.text.unicode < 128) // Bersekore is scared of unicode
	{
		buffer += static_cast<char>(event.text.unicode);
	}
}

std::string TextInput::getString() const
{
	return buffer;
}

void TextInput::clear()
{
	buffer.clear();
}

std::string TextInput::getAndClearString()
{
	std::string ret = getString();
	clear();
	return ret;
}

void TextInput::shiftleft(unsigned int how_many)
{
	if (buffer.size() < how_many + 1)
		return;

	std::string new_buf;
	new_buf = buffer.substr(how_many);
	buffer = new_buf;
}
