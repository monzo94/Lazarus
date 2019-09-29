#include <lazarus/Graphics/Window.h>
#include <lazarus/Random.h>

using namespace lz;

Window::Window(int width, int height, Color bg_color/*=Color::Black*/)
    : width(width)
    , height(height)
    , bg_color(bg_color)
{
}

void Window::load_tileset(const std::string &path, unsigned tile_size)
{
    tileset.load(path, tile_size);
    window.create(sf::VideoMode(tile_size * width, tile_size * height), "Lazarus");
}

void Window::render()
{
    window.clear(bg_color);
    unsigned tile_size = tileset.get_tile_size();
    if (tile_size == 0)
        return;  // No tileset loaded

    // Draw all the tiles
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            // TODO: for now print a random  sprite in the tileset
            std::vector<int> indices(tileset.get_num_tiles());
            std::iota(indices.begin(), indices.end(), 0);
            int id = Random::choice(indices);
            sf::Sprite &sprite = tileset.get_tile(id);
            sprite.setPosition(x * tile_size, y * tile_size);
            window.draw(sprite);
        }
    }
    window.display();
}

// TODO: delete main loop from here, it is only for testing
void Window::render_loop()
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
