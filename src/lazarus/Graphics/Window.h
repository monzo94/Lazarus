#pragma once

#include <lazarus/Graphics/Tileset.h>

namespace lz
{
using sf::Color;

class Window
{
public:
    // TODO: Layering/subwindow system
    Window(int width, int height, Color bgColor = Color::Black);

    void loadTileset(const std::string &path, unsigned tileSize);

    void render();

    void renderLoop();

private:
    // Dimensions in tiles of the window
    int width, height;
    // The window itself
    sf::RenderWindow window;
    // The tileset that the window uses to draw sprites
    Tileset tileset;
    // Tile ID to print at each position on render
    std::vector<int> buffer;
    Color bgColor;
};
}  // namespace lz
