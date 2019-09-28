#include <lazarus/Graphics/Tileset.h>

#include <lazarus/common.h>

using namespace lz;

Tileset::Tileset()
    : tileSize(0)
    , numTiles(0)
    , textureWidth(0)
    , textureHeight(0)
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
    textureWidth = imageSize.x;
    textureHeight = imageSize.y;
    if (textureWidth % size || textureHeight % size)
        throw __lz::LazarusException("The tilemap has wrong dimensions: " + path);

    // Clear the old tileset and load the new one
    numTiles = textureWidth * textureHeight / (size * size);
    tiles.clear();
    // Assign an id to each tile in reading order
    for (int id = 0; id < numTiles; ++id)
    {
        int col = id % (textureWidth / size);
        int row = id / (textureWidth / size);

        sf::IntRect spriteRect(col * size, row * size, size, size);
        tiles.emplace_back(texture, spriteRect);
    }
}

sf::Sprite &Tileset::getTile(int id)
{
    if (!isLoaded())
        throw __lz::LazarusException("No tileset loaded");
    if (id >= 0 && id < tiles.size())
        return tiles[id];
    return tiles[0];  // Return first tile as placeholder
}
