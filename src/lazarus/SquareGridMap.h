#pragma once

#include <vector>

namespace lz
{
/**
 * Component that determines an entity's position in a 2D grid.
 * 
 * This component can be used by any map that can be parametrized by
 * two discrete parameters, e.g. a square grid or hex grid maps.
 * For instance, it's what the SquareGridMap implementation uses.
 */
struct Position2D
{
    /**
     * Construct a position from the given coordinates.
     */
    Position2D(long x, long y);

    bool operator==(const Position2D& other) const;

    bool operator!=(const Position2D& other) const;

    bool operator<(const Position2D& other) const;

    long x, y;
};

/**
 * Defines a map consisting of square tiles in a rectangular grid.
 * 
 * The map has a fixed maximum width and height, which are set when constructing it.
 * The top-left corner of the map always corresponds to the coordinate (0, 0), and the
 * bottom-right corner would correspond to the coordinate (width - 1, height - 1).
 * The x axis has the positive direction going to the right, and the y axis has the positive
 * direction going down.
 * 
 * Each tile in the grid can be walkable and transparent.
 * 
 * A **walkable** tile is a tile in which entities that can move are allowed to enter,
 * and are the tiles considered in pathfinding algorithms. 
 * 
 * A **transparent** tile is a tile which allows light to pass through it. This is primarily
 * used by FOV algorithms, to determine which tiles an entity can see. A non-transparent tile
 * will be visible, but will block light, so no tiles behind it will be visible when casting
 * a light ray.
 */
class SquareGridMap
{
public:
    /**
     * Constructor for the map.
     * 
     * By default, all tiles will be set to not walkable
     * and not transparent.
     * 
     * @param width Maximum width of the map.
     * @param height Maximum height of the map.
     * @param diagonals Whether or not to consider diagonals as adjacent tiles.
     */
    SquareGridMap(unsigned long width, unsigned long height, bool diagonals=false);

    /**
     * Constructs a new map from the given prefab.
     * 
     * @param prefab Matrix representing a map, where a tile with 0 is a wall
     * and everything else is floor. The dimensions of the new map created
     * will coincide with the dimensions of the matrix. If the dimensions of
     * the matrix are not consistent (rows have different length), they will
     * be filled with walls to match the longest row.
     * @param diagonals Whether or not to consider diagonals as adjacent tiles.
     */
    SquareGridMap(const std::vector<std::vector<int>> &prefab, bool diagonals=false);

    /**
     * @return The width of the map.
     */
    unsigned long get_width() const;
    
    /**
     * @return The height of the map.
     */
    unsigned long get_height() const;

    /**
     * Returns whether the tile at the given position is walkable.
     * 
     * @param pos A 2D position.
     * 
     * @return `true` if the tile at the given position is walkable, `false` if it is
     * not walkable or if the position is out of the boundaries of the map.
     * 
     * @see is_walkable(long, long) const
     */
    bool is_walkable(const Position2D& pos) const;

    /**
     * Overloaded version of @ref is_walkable(const Position2D&) const
     * which takes the coordinates of the position as arguments.
     * 
     * @param x The x coordinate of a 2D position.
     * @param y The y coordinate of a 2D position.
     * 
     * @see is_walkable(const Position2D&) const
     */
    bool is_walkable(long x, long y) const;

    /**
     * Returns whether the tile at the given position is transparent.
     * 
     * @param pos A 2D position.
     * 
     * @return `true` if the tile at the given position is transparent, `false` if it is
     * not transparent or if the position is out of the boundaries of the map.
     * 
     * @see is_transparent(long, long) const
     */
    bool is_transparent(const Position2D& pos) const;

    /**
     * Overloaded version of @ref is_transparent(const Position2D&) const
     * which takes the coordinates of the position as arguments.
     * 
     * @param x The x coordinate of a 2D position.
     * @param y The y coordinate of a 2D position.
     * 
     * @see is_transparent(const Position2D&) const
     */
    bool is_transparent(long x, long y) const;

    /**
     * Returns whether the given position is within the boundaries of the map.
     * 
     * @param pos A 2D position.
     * 
     * @return `true` if the tile is within the boundaries of the map, that is,
     * `0 <= x < width` and `0 <= y < height`, and `false` otherwise.
     * 
     * @see is_out_of_bounds(long, long) const
     */
    bool is_out_of_bounds(const Position2D& pos) const;

    /**
     * Overloaded version of @ref is_out_of_bounds(const Position2D&) const
     * which takes the coordinates of the position as arguments.
     * 
     * @param x The x coordinate of a 2D position.
     * @param y The y coordinate of a 2D position.
     * 
     * @see is_out_of_bounds(const Position2D&) const
     */
    bool is_out_of_bounds(long x, long y) const;

    /**
     * Gets the cost of the tile at the given position.
     * 
     * @param pos A 2D position.
     * 
     * @throws LazarusException If the position is outside of the boundaries of the map,
     * or if the tile is not walkable.
     * 
     * @return The cost of accessing the tile from an adjacent tile.
     * 
     * @see get_cost(long, long) const
     */
    float get_cost(const Position2D& pos) const;

    /**
     * Overloaded version of @ref get_cost(const Position2D&) const
     * which takes the coordinates of the position as arguments.
     * 
     * @param x The x coordinate of a 2D position.
     * @param y The y coordinate of a 2D position.
     * 
     * @see get_cost(const Position2D&) const
     */
    float get_cost(long x, long y) const;

    /**
     * Sets the cost of the tile at the given position.
     * 
     * @param pos A 2D position.
     * @param cost The new cost of the tile.
     * 
     * @throws LazarusException If the position is outside of the boundaries of the map.
     * 
     * @see set_cost(long, long, float);
     */
    void set_cost(const Position2D& pos, float cost);

    /**
     * Overloaded version of @ref set_cost(const Position2D& pos, float cost) const
     * which takes the coordinates of the position as arguments.
     * 
     * @param x The x coordinate of a 2D position.
     * @param y The y coordinate of a 2D position.
     * 
     * @see set_cost(const Position2D& pos, float cost) const
     */
    void set_cost(long x, long y, float cost);

    /**
     * Changes the walkability of the tile at the given position.
     * 
     * If the tile was already walkable, nothing changes. If the tile was unwalkable,
     * it will be made walkable with a default cost of 1.
     * To make a tile walkable with a cost different than 1, the method set_cost() can
     * be used instead, as setting a cost greater than zero will make the tile walkable too.
     * 
     * @param pos A 2D position.
     * @param walkable The new walkability state for the tile.
     * 
     * @throws LazarusException If the position is outside of the boundaries of the map.
     * 
     * @see set_walkable(long, long, bool);
     */
    void set_walkable(const Position2D& pos, bool walkable);

    /**
     * Overloaded version of @ref set_walkable(const Position2D& pos, bool walkable) const
     * which takes the coordinates of the position as arguments.
     * 
     * @param x The x coordinate of a 2D position.
     * @param y The y coordinate of a 2D position.
     * 
     * @see set_walkable(const Position2D& pos, bool walkable) const
     */
    void set_walkable(long x, long y, bool walkable);

    /**
     * Changes the transparency of the tile at the given position.
     * 
     * @param pos A 2D position.
     * @param transparent The new transparency state for the tile.
     * 
     * @throws LazarusException If the position is outside of the boundaries of the map.
     * 
     * @see set_transparency(long, long, bool)
     */
    void set_transparency(const Position2D& pos, bool transparent);

    /**
     * Overloaded version of @ref set_transparency(const Position2D& pos, bool transparent) const
     * which takes the coordinates of the position as arguments.
     * 
     * @param x The x coordinate of a 2D position.
     * @param y The y coordinate of a 2D position.
     * 
     * @see set_transparency(const Position2D& pos, bool transparent) const
     */
    void set_transparency(long x, long y, bool transparent);

    /**
     * Returns a vector with the walkable tiles adjacent to the tile at the given position.
     * 
     * Only tiles within the boundaries of the map and that are walkable will be considered
     * as neighbours.
     * If the map allows diagonals (see constructor), tiles positioned diagonally to the
     * tile at the given position will also be considered.
     * 
     * @param pos A 2D position.
     * 
     * @throws LazarusException If the position is out of bounds.
     * 
     * @return A list of walkable tiles adjacent to the tile at position `pos`.
     * 
     * @see neighbours(long, long) const
     */
    virtual std::vector<Position2D> neighbours(const Position2D& pos) const;

    /**
     * Overloaded version of @ref neighbours(const Position2D& pos) const
     * which takes the coordinates of the position as arguments.
     * 
     * @param x The x coordinate of a 2D position.
     * @param y The y coordinate of a 2D position.
     * 
     * @see neighbours(const Position2D& pos) const
     */
    virtual std::vector<Position2D> neighbours(long x, long y) const;

    /**
     * Makes a rectangular area of tiles walkable and transparent.
     * 
     * By default, the cost of the tiles in the room will all be set to 1, but it
     * can be changed to another value.
     * 
     * @param top_left Top-left position of the tile of the rectangular room.
     * @param bottom_right Bottom-right position of the tile of the rectangular room.
     * @param cost Cost that will be given to all the tiles in the created room.
     */
    void carve_room(const Position2D &top_left,
                    const Position2D &bottom_right,
                    float cost = 1);

private:
    bool diagonals = false;
    unsigned long width, height;
    std::vector<float> costs;
    std::vector<bool> transparencies;
};
}  // namespace lz

namespace __lz  // Meant only for internal use
{
void throw_out_of_bounds_exception(const lz::Position2D& pos);
}  // namespace __lz
