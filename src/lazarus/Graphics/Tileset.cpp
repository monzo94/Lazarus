#include <lazarus/Graphics/Tileset.h>

#include <lazarus/common.h>

using namespace lz;

Tileset::Tileset()
    : tile_size(0)
    , num_tiles(0)
    , texture_width(0)
    , texture_height(0)
{
}

Tileset::Tileset(const std::string &path, const unsigned size)
{
    load(path, size);
}

bool Tileset::is_loaded() const
{
    return !tiles.empty() && tile_size != 0;
}

unsigned Tileset::get_tile_size() const
{
    return tile_size;
}

unsigned Tileset::get_num_tiles() const
{
    return num_tiles;
}

void Tileset::load(const std::string &path, const unsigned size)
{
    tile_size = size;

    // Load the image file
    if (!texture.loadFromFile(path))
        throw __lz::LazarusException("Could not load resource at path: " + path);

    // Tileset's width and height has to be divisible by the tile size
    // TODO: Allow non-square sizes
    sf::Vector2u image_size = texture.getSize();
    texture_width = image_size.x;
    texture_height = image_size.y;
    if (texture_width % size || texture_height % size)
        throw __lz::LazarusException("The tilemap has wrong dimensions: " + path);

    // Clear the old tileset and load the new one
    num_tiles = texture_width * texture_height / (size * size);
    tiles.clear();
    // Assign an id to each tile in reading order
    for (int id = 0; id < num_tiles; ++id)
    {
        int col = id % (texture_width / size);
        int row = id / (texture_width / size);

        sf::IntRect sprite_rect(col * size, row * size, size, size);
        tiles.emplace_back(texture, sprite_rect);
    }
}

sf::Sprite &Tileset::get_tile(int id)
{
    if (!is_loaded())
        throw __lz::LazarusException("No tileset loaded");
    if (id >= 0 && id < tiles.size())
        return tiles[id];
    return tiles[0];  // Return first tile as placeholder
}
