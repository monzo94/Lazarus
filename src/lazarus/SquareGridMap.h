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
    Position2D(int x, int y);
    int x, y;
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
     * @param width Maximum width of the map.
     * @param height Maximum height of the map.
     * @param diagonals Whether or not to consider diagonals as adjacent tiles.
     */
    SquareGridMap(unsigned width, unsigned height, bool diagonals=false);

    /**
     * @return The width of the map.
     */
    unsigned getWidth() const;
    
    /**
     * @return The height of the map.
     */
    unsigned getHeight() const;

    /**
     * Returns whether the tile at the given position is walkable.
     * 
     * @param pos A 2D position.
     * 
     * @return `true` if the tile at the given position is walkable, `false` if it is
     * not walkable or if the position is out of the boundaries of the map.
     * 
     * @see isWalkable(int, int) const
     */
    bool isWalkable(const Position2D& pos) const;

    /**
     * Overloaded version of @ref isWalkable(const Position2D&) const
     * which takes the coordinates of the position as arguments.
     * 
     * @param x The x coordinate of a 2D position.
     * @param y The y coordinate of a 2D position.
     * 
     * @see isWalkable(const Position2D&) const
     */
    bool isWalkable(int x, int y) const;

    /**
     * Returns whether the tile at the given position is transparent.
     * 
     * @param pos A 2D position.
     * 
     * @return `true` if the tile at the given position is transparent, `false` if it is
     * not transparent or if the position is out of the boundaries of the map.
     * 
     * @see isTransparent(int, int) const
     */
    bool isTransparent(const Position2D& pos) const;

    /**
     * Overloaded version of @ref isTransparent(const Position2D&) const
     * which takes the coordinates of the position as arguments.
     * 
     * @param x The x coordinate of a 2D position.
     * @param y The y coordinate of a 2D position.
     * 
     * @see isTransparent(const Position2D&) const
     */
    bool isTransparent(int x, int y) const;

    /**
     * Returns whether the given position is within the boundaries of the map.
     * 
     * @param pos A 2D position.
     * 
     * @return `true` if the tile is within the boundaries of the map, that is,
     * `0 <= x < width` and `0 <= y < height`, and `false` otherwise.
     * 
     * @see isOutOfBounds(int, int) const
     */
    bool isOutOfBounds(const Position2D& pos) const;

    /**
     * Overloaded version of @ref isOutOfBounds(const Position2D&) const
     * which takes the coordinates of the position as arguments.
     * 
     * @param x The x coordinate of a 2D position.
     * @param y The y coordinate of a 2D position.
     * 
     * @see isOutOfBounds(const Position2D&) const
     */
    bool isOutOfBounds(int x, int y) const;

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
     * @see getCost(int, int) const
     */
    float getCost(const Position2D& pos) const;

    /**
     * Overloaded version of @ref getCost(const Position2D&) const
     * which takes the coordinates of the position as arguments.
     * 
     * @param x The x coordinate of a 2D position.
     * @param y The y coordinate of a 2D position.
     * 
     * @see getCost(const Position2D&) const
     */
    float getCost(int x, int y) const;

    /**
     * Sets the cost of the tile at the given position.
     * 
     * @param pos A 2D position.
     * @param cost The new cost of the tile.
     * 
     * @throws LazarusException If the position is outside of the boundaries of the map.
     * 
     * @see setCost(int, int, float);
     */
    void setCost(const Position2D& pos, float cost);

    /**
     * Overloaded version of @ref setCost(const Position2D& pos, float cost) const
     * which takes the coordinates of the position as arguments.
     * 
     * @param x The x coordinate of a 2D position.
     * @param y The y coordinate of a 2D position.
     * 
     * @see setCost(const Position2D& pos, float cost) const
     */
    void setCost(int x, int y, float cost);

    /**
     * Changes the walkability of the tile at the given position.
     * 
     * If the tile was already walkable, nothing changes. If the tile was unwalkable,
     * it will be made walkable with a default cost of 1.
     * To make a tile walkable with a cost different than 1, the method setCost() can
     * be used instead, as setting a cost greater than zero will make the tile walkable too.
     * 
     * @param pos A 2D position.
     * @param walkable The new walkability state for the tile.
     * 
     * @throws LazarusException If the position is outside of the boundaries of the map.
     * 
     * @see setWalkable(int, int, bool);
     */
    void setWalkable(const Position2D& pos, bool walkable);

    /**
     * Overloaded version of @ref setWalkable(const Position2D& pos, bool walkable) const
     * which takes the coordinates of the position as arguments.
     * 
     * @param x The x coordinate of a 2D position.
     * @param y The y coordinate of a 2D position.
     * 
     * @see setWalkable(const Position2D& pos, bool walkable) const
     */
    void setWalkable(int x, int y, bool walkable);

    /**
     * Changes the transparency of the tile at the given position.
     * 
     * @param pos A 2D position.
     * @param transparent The new transparency state for the tile.
     * 
     * @throws LazarusException If the position is outside of the boundaries of the map.
     * 
     * @see setTransparency(int, int, bool)
     */
    void setTransparency(const Position2D& pos, bool transparent);

    /**
     * Overloaded version of @ref setTransparency(const Position2D& pos, bool transparent) const
     * which takes the coordinates of the position as arguments.
     * 
     * @param x The x coordinate of a 2D position.
     * @param y The y coordinate of a 2D position.
     * 
     * @see setTransparency(const Position2D& pos, bool transparent) const
     */
    void setTransparency(int x, int y, bool transparent);

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
     * @see neighbours(int, int) const
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
    virtual std::vector<Position2D> neighbours(int x, int y) const;

    /**
     * Makes a rectangular area of tiles walkable and transparent.
     * 
     * By default, the cost of the tiles in the room will all be set to 1, but it
     * can be changed to another value.
     * 
     * @param topLeft Top-left position of the tile of the rectangular room.
     * @param bottomRight Bottom-right position of the tile of the rectangular room.
     * @param cost Cost that will be given to all the tiles in the created room.
     */
    void carveRoom(const Position2D& topLeft,
                   const Position2D& bottomRight,
                   float cost=1);

private:
    bool diagonals = false;
    unsigned width, height;
    std::vector<float> costs;
    std::vector<bool> transparencies;
};
}  // namespace lz

namespace __lz  // Meant only for internal use
{
void throwOutOfBoundsException(const lz::Position2D& pos);
}  // namespace __lz
