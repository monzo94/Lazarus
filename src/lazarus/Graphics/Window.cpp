#include <lazarus/Graphics/Window.h>

using namespace lz;

Window::Window(int width, int height, Color bg_color/*=Color::Black*/)
    : width(width)
    , height(height)
    , bg_color(bg_color)
    , buffer(width * height)
{
    clear_buffer();
}

int Window::get_width() const
{
    return width;
}

int Window::get_height() const
{
    return height;
}

void Window::load_tileset(const std::string &path, unsigned tile_size)
{
    tileset.load(path, tile_size);
    window.create(sf::VideoMode(tile_size * width, tile_size * height), "Lazarus");
}

void Window::set_tile(const Position2D &pos, int tile_id, Color color)
{
    if (tile_id < 0 || tile_id >= tileset.get_num_tiles())
    {
        // Tile ID not valid
        return;
    }
    int buff_pos = pos.x + width * pos.y;
    if (buff_pos < 0 || buff_pos >= width * height)
    {
        // Position in window not valid
        return;
    }
    buffer[buff_pos] = std::make_pair(tile_id, color);
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
            int id;
            Color color;
            std::tie(id, color) = buffer[x + y * width];
            if (id == -1)
                continue;
            sf::Sprite &sprite = tileset.get_tile(id);
            sprite.setPosition(x * tile_size, y * tile_size);
            sprite.setColor(color);
            window.draw(sprite);
        }
    }
    window.display();
    clear_buffer();
}

bool Window::is_open() const
{
    return window.isOpen();
}

void Window::close()
{
    window.close();
}

bool Window::poll_event(Event &event)
{
    return window.pollEvent(event);
}

void Window::clear_buffer()
{
    std::fill(buffer.begin(), buffer.end(), std::make_pair(-1, Color::White));
}
