#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace lz
{
/**
 * The Tileset class manages tilesets loaded from a texture image.
 *
 * A tileset is just the result of slicing an image in a grid pattern,
 * where each cell in the grid is a tile that is stored as an individual
 * sprite.
 * To load a tileset, both the path of the image and the tile size need
 * to be provided.
 */
class Tileset
{
public:
    /**
     * Default constructor, which does not load a texture.
     */
    Tileset();

    /**
     * Constructor which loads a texture calling Tileset::load.
     */
    Tileset(const std::string &path);

    /**
     * Loads a texture from the image at the given path.
     *
     * The image must have a filename in the following format: name_widthxheight
     * For example, the my_texture_12x12.png has the correct format, with
     * name my_texture, width 12 and height 12.
     */
    void load(const std::string &path);

    /**
     * Return whether a valid texture has been loaded.
     */
    bool is_loaded() const;

    /**
     * Return the size of each tile in pixels.
     */
    unsigned get_tile_size() const;

    /**
     * Get the total number of tiles loaded from the texture.
     */
    unsigned get_num_tiles() const;

    /**
     * Get a tile from its id, which is its position in the texture
     * in reading order.
     */
    sf::Sprite &get_tile(int id);

    /**
     * Get a tile from its position (in tiles) in the texture.
     */
    sf::Sprite &get_tile(int x, int y);

private:
    // Currently loaded texture
    sf::Texture texture;
    unsigned texture_width, texture_height;
    std::string tileset_name;

    // Size per tile of the currently loaded texture (tiles are square)
    unsigned tile_size;

    // Number of tiles in the tileset
    unsigned num_tiles;

    // Hold the tileset currently loaded by the engine
    std::vector<sf::Sprite> tiles;
};
}  // namespace lz
