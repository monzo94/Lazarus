#pragma once

#include <lazarus/Graphics/Tileset.h>
#include <lazarus/SquareGridMap.h>

namespace lz
{
using sf::Color;
using sf::Event;

class Window
{
public:
    // TODO: Layering/subwindow system
    Window(int width, int height, Color bg_color = Color::Black);

    int get_width() const;
    int get_height() const;

    void load_tileset(const std::string &path, unsigned tile_size);

    void set_tile(const Position2D &pos, int tile_id);

    void render();

    bool is_open() const;

    void close();

    bool poll_event(Event &event);

protected:
    void clear_buffer();

private:
    // Dimensions in tiles of the window
    int width, height;
    // The window itself
    sf::RenderWindow window;
    // The tileset that the window uses to draw sprites
    Tileset tileset;
    // Tile ID to print at each position on render
    std::vector<int> buffer;
    Color bg_color;
};
}  // namespace lz
