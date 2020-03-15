#pragma once

#include <lazarus/Graphics/Tileset.h>
#include <lazarus/SquareGridMap.h>
#include <lazarus/common.h>

namespace lz
{
using sf::Color;
using sf::Event;

/**
 * Handles the main window of the game.
 *
 * The window is what will be shown to the user,
 * and where all the sprites are rendered.
 *
 * Coordinates in the window are given in tiles, where
 * (0, 0) is the top-left corner, with the x-axis
 * increasing to the right and the y-axis increasing
 * when going down.
 */
class Window : public sf::RenderWindow
{
public:
    // TODO: Layering/subwindow system
    /**
     * Default constructor.
     *
     * The actual window is not created until a tileset is loaded.
     */
    Window();

    /**
     * Initializes the window with the given dimensions in tiles
     * and background color.
     */
    void init(Tileset &tileset,
              int width,
              int height,
              std::string title,
              Color bg_color = Color::Black);

    /**
     * Loads the font at the given path.
     *
     * If the loading fails, an exception will be thrown.
     */
    void load_font(const std::string &path);

    /**
     * Returns whether the window has been initialized with a valid
     * tileset and dimensions.
     */
    bool is_initialized() const;

    /**
     * Returns the width in tiles of the window.
     */
    int get_width() const;

    /**
     * Returns the height in tiles of the window.
     */
    int get_height() const;

    /**
     * Draws the tile with the given ID from the tileset at the
     * given position, in window coordinates.
     *
     * If a tile was already drawn in the given position before
     * flushing the window, it will be drawn on top.
     *
     * @param pos Position in window coordinates where the tile should
     * be drawn.
     * @param tile_id ID of the tile in the tileset
     * @param color Color multiplier to use for the tile
     * to be drawn. By default, it is white, which preserves
     * the original color.
     */
    void set_tile(const Position2D &pos, int tile_id, Color color = Color::White);

    void draw_text(
        std::string msg, int x, int y, int size_px = 24, Color color = Color::White);
    void draw_text(sf::Text &text, int x, int y);

    /**
     * Display the current contents of the window and
     * flush the window buffer.
     */
    void render();

    /**
     * Return the currently loaded tileset, or a nullptr if
     * no tileset is loaded.
     */
    const Tileset *get_tileset() const;

private:
    // Dimensions in tiles of the window
    int width, height;
    // The currently loaded tileset
    Tileset *tileset;
    // The currently loaded font
    sf::Font font;
    // Background color for when a tile is not rendered
    Color bg_color;
    // Whether the window has been initialized with correct values
    bool initialized;
};
}  // namespace lz
