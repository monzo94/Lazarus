#pragma once

#include <SFML/Graphics/Text.hpp>

/**
 * Extension of SFML's Text class with line wrapping and other utilities.
 */
class TextBox : public sf::Text
{
public:
    TextBox(int width);
    void set_line_wrap(bool state);
    void set_width(int size);

    void setString(const sf::String &string);

private:
    int width;
    bool wrap;
};