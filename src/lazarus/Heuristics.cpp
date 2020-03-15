#include <lazarus/Heuristics.h>

#include <cmath>

using namespace lz;

float lz::manhattan_distance(const Position2D &a, const Position2D &b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

float lz::euclidean_distance(const Position2D &a, const Position2D &b)
{
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return std::hypot(dx, dy);
}

float lz::chebyshev_distance(const Position2D &a, const Position2D &b)
{
    return std::max(std::abs(a.x - b.x), std::abs(a.y - b.y));
}

float lz::octile_distance(const Position2D &a, const Position2D &b)
{
    float dx = std::abs(a.x - b.x);
    float dy = std::abs(a.y - b.y);
    return std::sqrt(2) * std::min(dx, dy) + std::abs(dx - dy);
}
