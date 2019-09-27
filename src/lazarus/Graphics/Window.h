#pragma once

#include <lazarus/Graphics/Tileset.h>

namespace lz
{
class Window
{
public:
    // TODO: Layering/subwindow system
    Window(int width, int height);

    void loadTileset(const std::string &path, unsigned tileSize);

    void render();

    void renderLoop();

private:
    int width, height;
    sf::RenderWindow window;
    Tileset tileset;
    // Tile ID to print at each position on render
    std::vector<int> buffer;
};
}  // namespace lz
