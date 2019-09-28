#pragma once

#include <set>
#include "SquareGridMap.h"

namespace lz
{
/**
 * Cast a linear ray from the origin to the destination and return
 * the visible positions.
 * 
 * It uses a modification of Bresenham's algorithm to always calculate
 * the path from origin to destination.
 * 
 * @param origin Where to cast the ray from.
 * @param dest Destination of the ray.
 * @param map Map that can be specified to check visibility. If no
 * map is specified, all positions in a line from origin to dest
 * will be returned. If a map is specified, only the positions which
 * are transparent will be added.
 * @param cancellable If set to true and a map is specified, the
 * algorithm will stop when the first non-transparent position is
 * encountered.
 */
std::vector<Position2D> castRay(const Position2D &origin,
                                const Position2D &dest,
                                const SquareGridMap *map = nullptr,
                                bool cancellable = true);

/**
 * Return whether origin has LOS of dest in the given map.
 */
bool LOS(const Position2D &origin,
         const Position2D &dest,
         const SquareGridMap &map);

/**
 * Return a vector of the positions that are visible from the origin at a given range
 * in the map.
 */
std::set<Position2D> simpleFov(const Position2D &origin, const int &range,
                               const SquareGridMap &map);
}  // namespace lz

namespace __lz
{
// Adds all the combinations of positions from the origin with the given
// offsets to the set of positions
void addOctants(const lz::Position2D &origin,
                const int &x,
                const int &y,
                std::set<lz::Position2D> &points);

// Return the circles on the circle at the given radius.
std::set<lz::Position2D> circle2D(const lz::Position2D &origin,
                                  const int &radius);
}
