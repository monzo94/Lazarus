#include <lazarus/SquareGridMap.h>
#include <lazarus/common.h>

#include <array>
#include <sstream>

using namespace lz;

void __lz::throw_out_of_bounds_exception(const Position2D &pos)
{
    std::stringstream msg;
    msg << "Position (" << pos.x << ", " << pos.y << ") is out of bounds.";
    throw __lz::LazarusException(msg.str());
}

Position2D::Position2D(long x, long y)
    : x(x)
    , y(y)
{
}

bool Position2D::operator==(const Position2D &other) const
{
    return x == other.x && y == other.y;
}

bool Position2D::operator!=(const Position2D &other) const
{
    return !(*this == other);
}

bool Position2D::operator<(const Position2D &other) const
{
    if (y == other.y)
        return x < other.x;
    return y < other.y;
}

SquareGridMap::SquareGridMap(unsigned long width, unsigned long height, bool diagonals)
    : diagonals(diagonals)
    , width(width)
    , height(height)
    , costs(width * height, -1.)
    , transparencies(width * height, false)
{
    if (width == 0 || height == 0)
        throw __lz::LazarusException("SquareGridMap width and height must be positive.");
}

SquareGridMap::SquareGridMap(const std::vector<std::vector<int>> &prefab, bool diagonals)
    : diagonals(diagonals)
{
    // Get dimensions
    height = prefab.size();
    width = 0;
    for (auto row : prefab)
        if (row.size() > width)
            width = row.size();

    if (width == 0 || height == 0)
        throw __lz::LazarusException("SquareGridMap width and height must be positive.");

    // Generate map from prefab
    // Tiles equal to 0 are walls (non-walkable, non-transparent)
    // The rest is walkable (with cost 1) and transparent
    costs = std::vector<float>(width * height, -1.);
    transparencies = std::vector<bool>(width * height, false);
    for (long y = 0; y < height; ++y)
    {
        for (long x = 0; x < width; ++x)
        {
            if (x < prefab[y].size() && prefab[y][x] != 0)
            {
                set_walkable({x, y}, true);
                set_transparency({x, y}, true);
            }
        }
    }
}

unsigned long SquareGridMap::get_width() const
{
    return width;
}

unsigned long SquareGridMap::get_height() const
{
    return height;
}

bool SquareGridMap::is_walkable(const Position2D &pos) const
{
    if (is_out_of_bounds(pos))
        return false;  // TODO: Log this case
    return costs[pos.y * width + pos.x] >= 0.;
}

bool SquareGridMap::is_walkable(long x, long y) const
{
    return is_walkable(Position2D(x, y));
}

bool SquareGridMap::is_transparent(const Position2D &pos) const
{
    if (is_out_of_bounds(pos))
        return false;  // TODO: Log this case
    return transparencies[pos.y * width + pos.x];
}

bool SquareGridMap::is_transparent(long x, long y) const
{
    return is_transparent(Position2D(x, y));
}

bool SquareGridMap::is_out_of_bounds(const Position2D &pos) const
{
    return is_out_of_bounds(pos.x, pos.y);
}

bool SquareGridMap::is_out_of_bounds(long x, long y) const
{
    return x < 0 || y < 0 || x >= width || y >= height;
}

float SquareGridMap::get_cost(const Position2D &pos) const
{
    if (is_out_of_bounds(pos))
    {
        // TODO: Log this case
        __lz::throw_out_of_bounds_exception(pos);
    }

    if (!is_walkable(pos))
    {
        std::stringstream msg;
        msg << "Tried to get cost of unwalkable tile at position (" << pos.x << ", "
            << pos.y << ").";
        throw __lz::LazarusException(msg.str());
    }

    return costs[pos.y * width + pos.x];
}

float SquareGridMap::get_cost(long x, long y) const
{
    return get_cost(Position2D(x, y));
}

std::vector<Position2D> SquareGridMap::neighbours(const Position2D &pos) const
{
    if (is_out_of_bounds(pos))
        // TODO: Log this case
        __lz::throw_out_of_bounds_exception(pos);

    std::vector<Position2D> result;
    long x = pos.x, y = pos.y;

    std::array<Position2D, 4> neighbour_positions{Position2D(x - 1, y),
                                                  Position2D(x + 1, y),
                                                  Position2D(x, y - 1),
                                                  Position2D(x, y + 1)};

    for (Position2D neighbour : neighbour_positions)
    {
        if (is_walkable(neighbour))
            result.push_back(neighbour);
    }

    if (diagonals)
    {
        std::array<Position2D, 4> diagonal_positions{Position2D(x - 1, y - 1),
                                                     Position2D(x + 1, y + 1),
                                                     Position2D(x + 1, y - 1),
                                                     Position2D(x - 1, y + 1)};

        for (Position2D neighbour : diagonal_positions)
        {
            if (is_walkable(neighbour))
                result.push_back(neighbour);
        }
    }

    return result;
}

std::vector<Position2D> SquareGridMap::neighbours(long x, long y) const
{
    return neighbours(Position2D(x, y));
}

void SquareGridMap::set_cost(const Position2D &pos, float cost)
{
    if (is_out_of_bounds(pos))
    {
        // TODO: Log this case
        __lz::throw_out_of_bounds_exception(pos);
    }

    costs[pos.y * width + pos.x] = cost;
}

void SquareGridMap::set_cost(long x, long y, float cost)
{
    set_cost(Position2D(x, y), cost);
}

void SquareGridMap::set_walkable(const Position2D &pos, bool walkable)
{
    if (is_walkable(pos) && walkable)
        return;  // do nothing
    set_cost(pos, walkable ? 1. : -1.);
}

void SquareGridMap::set_walkable(long x, long y, bool walkable)
{
    set_walkable(Position2D(x, y), walkable);
}

void SquareGridMap::set_transparency(const Position2D &pos, bool transparent)
{
    if (is_out_of_bounds(pos))
    {
        // TODO: Log this case
        __lz::throw_out_of_bounds_exception(pos);
    }

    transparencies[pos.y * width + pos.x] = transparent;
}

void SquareGridMap::set_transparency(long x, long y, bool transparent)
{
    set_transparency(Position2D(x, y), transparent);
}

void SquareGridMap::carve_room(const Position2D &top_left,
                               const Position2D &bottom_right,
                               float cost)
{
    for (long x = top_left.x; x <= bottom_right.x; ++x)
    {
        for (long y = top_left.y; y <= bottom_right.y; ++y)
        {
            Position2D pos(x, y);
            set_cost(pos, cost);
            set_transparency(pos, true);
        }
    }
}
