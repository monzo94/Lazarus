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
float manhattan_distance(const Position2D&, const Position2D&);

/**
 * Compute the Euclidean distance between two 2D positions.
 */
float euclidean_distance(const Position2D&, const Position2D&);

/**
 * Compute the Chebyshev distance between two 2D positions.
 */
float chebyshev_distance(const Position2D&, const Position2D&);

/**
 * Compute the Chebyshev distance between two 2D positions.
 */
float octile_distance(const Position2D&, const Position2D&);
}  // namespace lz