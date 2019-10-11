#pragma once

#include <lazarus/Graphics/Tileset.h>
#include <lazarus/SquareGridMap.h>

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
class Window
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
    void init(Tileset &tileset, int width, int height,
              std::string title, Color bg_color = Color::Black);

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
    void set_tile(const Position2D &pos, int tile_id, Color color=Color::White);

    /**
     * Display the current contents of the window and
     * flush the window buffer.
     */
    void render();

    /**
     * Return whether the window is open.
     */
    bool is_open() const;

    /**
     * Closes the window.
     */
    void close();

    /**
     * Gets the next even in the queue.
     * 
     * @param event Reference in which the next event
     * will be stored.
     */
    bool poll_event(Event &event);

private:
    // Dimensions in tiles of the window
    int width, height;
    // The window itself
    sf::RenderWindow window;
    // The tileset that the window uses to draw sprites
    Tileset *tileset;
    // Background color for when a tile is not rendered
    Color bg_color;
    // Whether the window has been initialized with correct values
    bool initialized;
};
}  // namespace lz
