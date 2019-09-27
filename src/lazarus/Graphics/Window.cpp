#include <lazarus/Graphics/Window.h>
#include <lazarus/Random.h>

using namespace lz;

Window::Window(int width, int height)
    : width(width)
    , height(height)
{
}

void Window::loadTileset(const std::string &path, unsigned tileSize)
{
    tileset.load(path, tileSize);
    window.create(sf::VideoMode(tileSize * width, tileSize * height), "Lazarus");
}

void Window::render()
{
    window.clear(sf::Color::Black);
    unsigned tileSize = tileset.getTileSize();
    if (tileSize == 0)
        return;  // No tileset loaded

    // Draw all the tiles
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            // TODO: for now print a random  sprite in the tileset
            std::vector<int> indices(tileset.getNumTiles());
            std::iota(indices.begin(), indices.end(), 0);
            int id = Random::choice(indices);
            sf::Sprite &sprite = tileset.getTile(id);
            sprite.setPosition(x * tileSize, y * tileSize);
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
