#include <lazarus/FOV.h>
#include <lazarus/common.h>

#include <algorithm>
#include <cmath>

using namespace lz;

std::vector<Position2D> lz::cast_ray(const Position2D &origin,
                                     const Position2D &dest,
                                     const SquareGridMap *map,
                                     int max_dist,
                                     bool cancellable)
{
    // Use modified Bresenham's algorithm to cast ray
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

        // If ray goes out of bounds, stop the cast
        if (map->is_out_of_bounds(pos))
            break;

        bool transparent = !map || map->is_transparent(pos);

        points.emplace_back(isSteep ? y : x, isSteep ? x : y);

        // TODO: pass an entity engine and check if there's a light blocking entity
        // in the current position too
        // If the ray got to a blocking tile or the maximum distance was reached, stop
        if ((cancellable && !transparent) || (max_dist > 0 && points.size() >= max_dist))
            break;

        error -= dy;
        if (error < 0)
        {
            // TODO: when line "breaks", check if we've crossed a diagonal of
            // non-transparent tiles, in which case it can be parametrized not
            // to continue the cast and consider the next tile non-transparent,
            // even if it is
            y += ystep;
            error += dx;
        }
    }

    return points;
}

bool lz::los(const Position2D &origin, const Position2D &dest, const SquareGridMap &map)
{
    // Cast a ray and check if it got to the destination
    auto ray = cast_ray(origin, dest, &map, true);
    return ray.back() == dest;
}

std::set<Position2D> lz::fov(const Position2D &origin,
                             const int &range,
                             const SquareGridMap &map,
                             FOV algorithm)
{
    switch (algorithm)
    {
    case FOV::Simple:
        return __lz::fov_simple(origin, range, map);
        break;
    default:
        throw __lz::LazarusException("FOV algorithm not implemented.");
    }
}

std::set<Position2D> lz::circle2D(const Position2D &origin, const int &radius)
{
    // Find circle positions for one octant and replicate to all other
    // octants using a modification of Bresenham's algorithm
    std::set<Position2D> circle;
    int x = 0, y = radius;
    int d = 3 - 2 * radius;
    __lz::add_octants(origin, x, y, circle);
    while (y >= x)
    {
        x++;
        if (d > 0)
        {
            y--;
            d += 4 * (x - y) + 10;
        }
        else
            d += 4 * x + 6;
        __lz::add_octants(origin, x, y, circle);
    }
    return circle;
}

void __lz::add_octants(const Position2D &origin,
                       const long &x,
                       const long &y,
                       std::set<Position2D> &points)
{
    int xc = origin.x, yc = origin.y;
    points.insert(Position2D(xc + x, yc + y));
    points.insert(Position2D(xc - x, yc + y));
    points.insert(Position2D(xc + x, yc - y));
    points.insert(Position2D(xc - x, yc - y));
    points.insert(Position2D(xc + y, yc + x));
    points.insert(Position2D(xc - y, yc + x));
    points.insert(Position2D(xc + y, yc - x));
    points.insert(Position2D(xc - y, yc - x));
}

std::set<Position2D> __lz::fov_simple(const Position2D &origin,
                                      const int &range,
                                      const SquareGridMap &map)
{
    std::set<Position2D> visible;
    // Origin is always visible
    visible.insert(origin);

    // Cast rays in all directions given by a square with the set range
    for (unsigned long idx = 0; idx <= range; ++idx)
    {
        // Shorten diagonals proportionally to make a "circle" FOV
        double slope_factor = 1. + ((std::sqrt(2) - 1.) * idx) / range;
        int max_cast_dist = std::ceil(range / slope_factor);
        std::set<Position2D> vertices;
        __lz::add_octants(origin, idx, range, vertices);
        for (auto pos : vertices)
        {
            auto ray = cast_ray(origin, pos, &map, max_cast_dist, true);
            // Add the entire line trajectory to visible,
            // except for out of bounds tiles
            std::copy_if(ray.begin(),
                         ray.end(),
                         std::inserter(visible, visible.begin()),
                         [&](auto ray_pos) { return !map.is_out_of_bounds(ray_pos); });
        }
    }
    return visible;
}
