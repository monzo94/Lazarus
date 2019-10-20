#include <lazarus/Graphics/Window.h>

using namespace lz;

Window::Window()
    : width(0)
    , height(0)
    , tileset(nullptr)
    , bg_color(Color::Black)
    , initialized(false)
{
}

void Window::init(Tileset &tileset_, int width_,
                  int height_, std::string title, Color bg_color_)
{
    tileset = &tileset_;
    width = width_;
    height = height_;
    bg_color = bg_color_;
    if (tileset->is_loaded() && width > 0 && height > 0)
    {
        unsigned tile_size = tileset->get_tile_size();
        window.create(sf::VideoMode(tile_size * width, tile_size * height), title);
        initialized = true;
    }
}

void Window::load_font(const std::string &path)
{
    if (!font.loadFromFile(path))
        throw __lz::LazarusException("Could not load font at path: " + path);
}

bool Window::is_initialized() const
{
    return initialized;
}

int Window::get_width() const
{
    return width;
}

int Window::get_height() const
{
    return height;
}

void Window::set_tile(const Position2D &pos, int tile_id, Color color)
{
    if (!tileset)
        return;

    if (tile_id < 0 || tile_id >= tileset->get_num_tiles())
    {
        // Tile ID not valid
        return;
    }
    sf::Sprite &sprite = tileset->get_tile(tile_id);
    unsigned tile_size = tileset->get_tile_size();
    sprite.setPosition(pos.x * tile_size, pos.y * tile_size);
    sprite.setColor(color);
    window.draw(sprite);
    return;
}

void Window::render()
{
    // Display contents and clear for future draws
    window.display();
    window.clear(bg_color);
}

bool Window::is_open() const
{
    return window.isOpen();
}

void Window::close()
{
    window.close();
}

const Tileset *Window::get_tileset() const
{
    return tileset;
}

bool Window::poll_event(Event &event)
{
    return window.pollEvent(event);
}
