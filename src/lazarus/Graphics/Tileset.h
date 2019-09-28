#pragma once

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

namespace lz
{
class Tileset
{
public:
    Tileset();
    Tileset(const std::string &path, const unsigned size);
    void load(const std::string &path, const unsigned size);
    bool isLoaded() const;
    unsigned getTileSize() const;
    unsigned getNumTiles() const;

    sf::Sprite &getTile(int id);

private:
    // Currently loaded texture
    sf::Texture texture;

    // Width and height per tile of the currently loaded texture
    unsigned tileSize;

    // Number of tiles in the tileset
    unsigned numTiles;

    // Hold the tileset currently loaded by the engine
    std::vector<sf::Sprite> tiles;
};
}  // namespace lz
