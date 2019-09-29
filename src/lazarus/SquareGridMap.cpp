#include <lazarus/SquareGridMap.h>

#include <array>
#include <sstream>

#include <lazarus/common.h>

using namespace lz;

void __lz::throw_out_of_bounds_exception(const Position2D &pos)
{
    std::stringstream msg;
    msg << "Position (" << pos.x << ", " << pos.y << ") is out of bounds.";
    throw __lz::LazarusException(msg.str());
}

Position2D::Position2D(int x, int y)
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

SquareGridMap::SquareGridMap(unsigned width, unsigned height, bool diagonals)
    : diagonals(diagonals)
    , width(width)
    , height(height)
    , costs(width * height, -1.)
    , transparencies(width * height, false)
{
    if (width == 0 || height == 0)
        throw __lz::LazarusException("SquareGridMap width and height must be positive.");
}

unsigned SquareGridMap::get_width() const
{
    return width;
}

unsigned SquareGridMap::get_height() const
{
    return height;
}

bool SquareGridMap::is_walkable(const Position2D &pos) const
{
    if (is_out_of_bounds(pos))
        return false;  // TODO: Log this case
    return costs[pos.y * width + pos.x] >= 0.;
}

bool SquareGridMap::is_walkable(int x, int y) const
{
    return is_walkable(Position2D(x, y));
}

bool SquareGridMap::is_transparent(const Position2D &pos) const
{
    if (is_out_of_bounds(pos))
        return false;  // TODO: Log this case
    return transparencies[pos.y * width + pos.x];
}

bool SquareGridMap::is_transparent(int x, int y) const
{
    return is_transparent(Position2D(x, y));
}

bool SquareGridMap::is_out_of_bounds(const Position2D &pos) const
{
    return is_out_of_bounds(pos.x, pos.y);
}

bool SquareGridMap::is_out_of_bounds(int x, int y) const
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
        msg << "Tried to get cost of unwalkable tile at position ("
            << pos.x << ", " << pos.y << ").";
        throw __lz::LazarusException(msg.str());
    }

    return costs[pos.y * width + pos.x];
}

float SquareGridMap::get_cost(int x, int y) const
{
    return get_cost(Position2D(x, y));
}

std::vector<Position2D> SquareGridMap::neighbours(const Position2D &pos) const
{
    if (is_out_of_bounds(pos))
        // TODO: Log this case
        __lz::throw_out_of_bounds_exception(pos);

    std::vector<Position2D> result;
    int x = pos.x, y = pos.y;

    std::array<Position2D, 4> neighbour_positions{
        Position2D(x - 1, y),
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
        std::array<Position2D, 4> diagonal_positions{
            Position2D(x - 1, y - 1),
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

std::vector<Position2D> SquareGridMap::neighbours(int x, int y) const
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

void SquareGridMap::set_cost(int x, int y, float cost)
{
    set_cost(Position2D(x, y), cost);
}

void SquareGridMap::set_walkable(const Position2D &pos, bool walkable)
{
    if (is_walkable(pos) && walkable)
        return;  // do nothing
    set_cost(pos, walkable ? 1. : -1.);
}

void SquareGridMap::set_walkable(int x, int y, bool walkable)
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

void SquareGridMap::set_transparency(int x, int y, bool transparent)
{
    set_transparency(Position2D(x, y), transparent);
}

void SquareGridMap::carve_room(const Position2D &top_left,
                               const Position2D &bottom_right,
                               float cost)
{
    for (int x = top_left.x; x <= bottom_right.x; ++x)
    {
        for (int y = top_left.y; y <= bottom_right.y; ++y)
        {
            Position2D pos(x, y);
            set_cost(pos, cost);
            set_transparency(pos, true);
        }
    }
}
