#include <lazarus/Graphics/Tileset.h>

#include <lazarus/common.h>

using namespace lz;

Tileset::Tileset()
    : tileSize(0)
    , numTiles(0)
{
}

Tileset::Tileset(const std::string &path, const unsigned size)
{
    load(path, size);
}

bool Tileset::isLoaded() const
{
    return !tiles.empty() && tileSize != 0;
}

unsigned Tileset::getTileSize() const
{
    return tileSize;
}

unsigned Tileset::getNumTiles() const
{
    return numTiles;
}

void Tileset::load(const std::string &path, const unsigned size)
{
    tileSize = size;

    // Load the image file
    if (!texture.loadFromFile(path))
        throw __lz::LazarusException("Could not load resource at path: " + path);

    // Tileset's width and height has to be divisible by the tile size
    // TODO: Allow non-square sizes
    sf::Vector2u imageSize = texture.getSize();
    if (imageSize.x % size || imageSize.y % size)
        throw __lz::LazarusException("The tilemap has wrong dimensions: " + path);

    // Clear the old tileset and load the new one
    numTiles = imageSize.x * imageSize.y / (size * size);
    tiles.clear();
    // Assign an id to each tile in reading order
    for (int id = 0; id < numTiles; ++id)
    {
        int col = id % (imageSize.x / size);
        int row = id / (imageSize.x / size);

        sf::IntRect spriteRect(col * size, row * size, size, size);
        tiles.emplace_back(texture, spriteRect);
    }
}

sf::Sprite& Tileset::getTile(int id)
{    
    return tiles[id];
}
