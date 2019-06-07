#pragma once

#include <functional>

#include <lazarus/SquareGridMap.h>

namespace lz
{
template <typename Position>
using Heuristic = std::function<float(const Position&, const Position&)>;

/**
 * Compute the Manhattan distance between two 2D positions.
 */
float manhattanDistance(const Position2D&, const Position2D&);

/**
 * Compute the Euclidean distance between two 2D positions.
 */
float euclideanDistance(const Position2D&, const Position2D&);

/**
 * Compute the Chebyshev distance between two 2D positions.
 */
float chebyshevDistance(const Position2D&, const Position2D&);

/**
 * Compute the Chebyshev distance between two 2D positions.
 */
float octileDistance(const Position2D&, const Position2D&);
}  // namespace lz