#include <lazarus/Graphics/Tileset.h>
#include <lazarus/common.h>
#include <filesystem>
#include <regex>

using namespace lz;

Tileset::Tileset()
    : tile_width(0)
    , tile_height(0)
    , num_tiles(0)
    , texture_width(0)
    , texture_height(0)
{
}

Tileset::Tileset(const std::string &path)
{
    load(path);
}

bool Tileset::is_loaded() const
{
    return !tiles.empty() && !tileset_name.empty();
}

unsigned Tileset::get_tile_width() const
{
    return tile_width;
}

unsigned Tileset::get_tile_height() const
{
    return tile_height;
}

unsigned Tileset::get_num_tiles() const
{
    return num_tiles;
}

void Tileset::load(const std::string &path)
{
    // Get tileset name and size from the filename
    std::string filename_without_ext{std::filesystem::path(path).stem()};
    std::regex re("(\\w+)_(\\d+)x(\\d+)");
    std::smatch match;
    if (std::regex_search(filename_without_ext, match, re) && match.size() > 3)
    {
        // match[0] is the total match
        // 1, 2 and 3 contain each of the groups
        tileset_name = match[1].str();
        tile_width = std::atoi(match[2].str().c_str());
        tile_height = std::atoi(match[3].str().c_str());
    }
    else
        throw __lz::LazarusException("Texture filename is not correctly formatted");
    

    // Load the image file
    if (!texture.loadFromFile(path))
        throw __lz::LazarusException("Could not load resource at path: " + path);

    // Tileset's width and height has to be divisible by the tile size
    // TODO: Allow non-square sizes
    sf::Vector2u image_size = texture.getSize();
    texture_width = image_size.x;
    texture_height = image_size.y;
    if (texture_width % tile_width || texture_height % tile_height)
        throw __lz::LazarusException("The tilemap has wrong dimensions: " + path);

    // Clear the old tileset and load the new one
    num_tiles = texture_width * texture_height / (tile_width * tile_height);
    tiles.clear();
    // Assign an id to each tile in reading order
    for (int id = 0; id < num_tiles; ++id)
    {
        int col = id % (texture_width / tile_width);
        int row = id / (texture_width / tile_height);

        sf::IntRect sprite_rect(col * tile_width, row * tile_height,
                                tile_width, tile_height);
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

sf::Sprite &Tileset::get_tile(int x, int y)
{
    if (!is_loaded())
        throw __lz::LazarusException("No tileset loaded");
    return get_tile(x + y * texture_width / tile_width);
}
