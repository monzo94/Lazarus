#include <lazarus/FOV.h>
#include <algorithm>
#include <cmath>

using namespace lz;

std::vector<Position2D> lz::castRay(const Position2D &origin,
                                    const Position2D &dest,
                                    const SquareGridMap *map,
                                    bool cancellable)
{
    // Use Bresenham's algorithm to cast ray
    std::vector<Position2D> points;
    int x0 = origin.x, y0 = origin.y, x1 = dest.x, y1 = dest.y;

    bool isSteep = std::abs(y1 - y0) > std::abs(x1 - x0);
    // If the line is steep, rotate the line
    if (isSteep)
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);

    int error = dx / 2;
    int ystep = y0 < y1 ? 1 : -1;
    int xstep = x0 < x1 ? 1 : -1;

    // Iterate over bounding box generating points between start and end
    int y = y0;
    for (int x = x0; x != x1 + xstep; x += xstep)
    {
        Position2D pos{isSteep ? y : x, isSteep ? x : y};
        bool transparent = !map || map->isTransparent(pos);

        if (transparent)
            points.emplace_back(isSteep ? y : x, isSteep ? x : y);

        if (cancellable && !transparent)
            break;

        error -= dy;
        if (error < 0)
        {
            y += ystep;
            error += dx;
        }
    }

    return points;
}

bool lz::LOS(const Position2D &origin,
             const Position2D &dest,
             const SquareGridMap &map)
{
    // Cast a ray and check if it got to the destination
    auto ray = castRay(origin, dest, &map, true);
    return ray.back() == dest;
}

std::set<Position2D> lz::simpleFov(const Position2D &origin, const int &range,
                                   const SquareGridMap &map)
{
    std::set<Position2D> visible;
    // Origin is always visible
    visible.insert(origin);

    // Cast rays in all directions given by a square with the set range
    // TODO: Use a "circle" instead of a square so that the range in the
    // diagonals is not larger
    for (int i = -range; i < range; ++i)
    {
        std::vector<Position2D> vertices{Position2D(origin.x + i, origin.y - range),
                                         Position2D(origin.x + i, origin.y + range),
                                         Position2D(origin.x - range, origin.y + i),
                                         Position2D(origin.x + range, origin.y + i)};
        for (auto pos : vertices)
        {
            auto ray = castRay(origin, pos, &map, true);
            std::copy(ray.begin(), ray.end(),
                      std::inserter(visible, visible.begin()));
        }
    }

    return visible;
}