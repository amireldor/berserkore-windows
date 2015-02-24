#ifndef TEXT_INPUT_HPP
#define TEXT_INPUT_HPP

#include <string>
#include <SFML/Window.hpp>

namespace bk
{

/**
 * Will read events you give it, filter the TextEnetered events, and help you
 * get beautiful strings from the user.
 */
class TextInput
{
public:
	/**
	 * Will check event.type, so if you like premature optimization,
	 * call `processTextEntered()`.
	 */
	void processEvent(const sf::Event&);
	void processTextEntered(const sf::Event&);

	std::string getString() const;
	void clear();
	std::string getAndClearString();

	/**
	 * Discard characters at the beginning of the buffer.
	 */
	void shiftleft(unsigned int how_many = 1);

private:
	std::string buffer;
};

}

#endif
