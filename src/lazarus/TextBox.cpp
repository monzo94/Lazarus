#include <lazarus/TextBox.h>

TextBox::TextBox(int width)
    : width(width)
{
}

void TextBox::set_line_wrap(bool state)
{
    wrap = state;
}

void TextBox::set_width(int size)
{
    width = size;
}

void TextBox::setString(const sf::String &string)
{
    // TODO: Do line wrapping if necessary, changing some spaces into newlines
    unsigned char_size = getCharacterSize();
    
    sf::Text::setString(string);
}
