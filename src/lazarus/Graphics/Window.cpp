#include <lazarus/Graphics/Window.h>

using namespace lz;

Window::Window(int width, int height)
    : width(width)
    , height(height)
{
    // TODO: hardcoded for testing; make parametrizable
    window.create(sf::VideoMode(12 * width, 12 * height), "Lazarus");
    tileset.load("/home/dani/Documents/Lazarus/res/dejavu12x12.png", 12);
}

void Window::render()
{
    window.clear(sf::Color::Black);
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            // TODO: for now only printing first sprite in the tileset
            int id = 1;
            sf::Sprite &sprite = tileset.getTile(id);
            // TODO: Use tile size
            sprite.setPosition(x * 12, y * 12);
            window.draw(sprite);
        }
    }
    window.display();
}

// TODO: delete main loop from here, it is only for testing
void Window::renderLoop()
{
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        render();
    }
}
