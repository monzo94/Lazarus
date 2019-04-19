#include <lazarus/SquareGridMap.h>

#include <array>
#include <sstream>

#include <lazarus/common.h>

using namespace lz;

void __lz::throwOutOfBoundsException(const Position2D& pos)
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

bool Position2D::operator==(const Position2D& other)
{
    return x == other.x && y == other.y;
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

unsigned SquareGridMap::getWidth() const
{
    return width;
}

unsigned SquareGridMap::getHeight() const
{
    return height;
}

bool SquareGridMap::isWalkable(const Position2D& pos) const
{
    if (isOutOfBounds(pos))
        return false;  // TODO: Log this case
    return costs[pos.y * width + pos.x] >= 0.;
}

bool SquareGridMap::isWalkable(int x, int y) const
{
    return isWalkable(Position2D(x, y));
}

bool SquareGridMap::isTransparent(const Position2D& pos) const
{
    if (isOutOfBounds(pos))
        return false;  // TODO: Log this case
    return transparencies[pos.y * width + pos.x];
}

bool SquareGridMap::isTransparent(int x, int y) const
{
    return isTransparent(Position2D(x, y));
}

bool SquareGridMap::isOutOfBounds(const Position2D& pos) const
{
    return isOutOfBounds(pos.x, pos.y);
}

bool SquareGridMap::isOutOfBounds(int x, int y) const
{
    return x < 0 || y < 0 || x >= width || y >= height;
}

float SquareGridMap::getCost(const Position2D& pos) const
{
    if (isOutOfBounds(pos))
    {
        __lz::throwOutOfBoundsException(pos);  // TODO: Log this case
    }

    if (!isWalkable(pos))
    {
        std::stringstream msg;
        msg << "Tried to get cost of unwalkable tile at position ("
            << pos.x << ", " << pos.y << ").";
        throw __lz::LazarusException(msg.str());
    }

    return costs[pos.y * width + pos.x];
}

float SquareGridMap::getCost(int x, int y) const
{
    return getCost(Position2D(x, y));
}

std::vector<Position2D> SquareGridMap::neighbours(const Position2D& pos) const
{
    if (isOutOfBounds(pos))
        __lz::throwOutOfBoundsException(pos);  // TODO: Log this case

    std::vector<Position2D> result;
    int x = pos.x, y = pos.y;

    std::array<Position2D, 4> neighbourPositions{
        Position2D(x - 1, y),
        Position2D(x + 1, y),
        Position2D(x, y - 1),
        Position2D(x, y + 1)
    };

    for (Position2D neighbour : neighbourPositions)
    {
        if (isWalkable(neighbour))
            result.push_back(neighbour);
    }

    if (diagonals)
    {
        std::array<Position2D, 4> diagonalPositions{
            Position2D(x - 1, y - 1),
            Position2D(x + 1, y + 1),
            Position2D(x + 1, y - 1),
            Position2D(x - 1, y + 1)
        };

        for (Position2D neighbour : diagonalPositions)
        {
            if (isWalkable(neighbour))
                result.push_back(neighbour);
        }
    }
    
    return result;
}

std::vector<Position2D> SquareGridMap::neighbours(int x, int y) const
{
    return neighbours(Position2D(x, y));
}

void SquareGridMap::setCost(const Position2D& pos, float cost)
{
    if (isOutOfBounds(pos))
    {
        __lz::throwOutOfBoundsException(pos);  // TODO: Log this case
    }

    costs[pos.y * width + pos.x] = cost;
}

void SquareGridMap::setCost(int x, int y, float cost)
{
    setCost(Position2D(x, y), cost);
}

void SquareGridMap::setWalkable(const Position2D& pos, bool walkable)
{
    if (isWalkable(pos) && walkable)
        return;  // do nothing
    setCost(pos, walkable ? 1. : -1.);
}

void SquareGridMap::setWalkable(int x, int y, bool walkable)
{
    setWalkable(Position2D(x, y), walkable);
}

void SquareGridMap::setTransparency(const Position2D& pos, bool transparent)
{
    if (isOutOfBounds(pos))
    {
        __lz::throwOutOfBoundsException(pos);  // TODO: Log this case
    }

    transparencies[pos.y * width + pos.x] = transparent;
}

void SquareGridMap::setTransparency(int x, int y, bool transparent)
{
    setTransparency(Position2D(x, y), transparent);
}

void SquareGridMap::carveRoom(const Position2D& topLeft,
                              const Position2D& bottomRight,
                              float cost)
{
    for (int x = topLeft.x; x <= bottomRight.x; ++x)
    {
        for (int y = topLeft.y; y <= bottomRight.y; ++y)
        {
            Position2D pos(x, y);
            setCost(pos, cost);
            setTransparency(pos, true);
        }
    }
}
