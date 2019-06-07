#include <lazarus/Heuristics.h>

#include <cmath>

using namespace lz;

float lz::manhattanDistance(const Position2D& a, const Position2D& b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

float lz::euclideanDistance(const Position2D& a, const Position2D& b)
{
    int dx = a.x - b.x,
        dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

float lz::chebyshevDistance(const Position2D& a, const Position2D& b)
{
    return std::max(std::abs(a.x - b.x), std::abs(a.y - b.y));
}

float lz::octileDistance(const Position2D& a, const Position2D& b)
{
    float dx = std::abs(a.x - b.x);
    float dy = std::abs(a.y - b.y);
    return std::sqrt(2) * std::min(dx, dy) + std::abs(dx - dy);
}
