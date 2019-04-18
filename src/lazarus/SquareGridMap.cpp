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
    return costs[pos.y * width + pos.x] < 0.;
}

bool SquareGridMap::isTransparent(const Position2D& pos) const
{
    if (isOutOfBounds(pos))
        return false;  // TODO: Log this case
    return transparencies[pos.y * width + pos.x];
}

bool SquareGridMap::isOutOfBounds(const Position2D& pos) const
{
    return pos.x < 0 || pos.y < 0 || pos.x >= width || pos.y >= height;
}

float SquareGridMap::getCost(const Position2D& pos) const
{
    if (isOutOfBounds(pos))
    {
        __lz::throwOutOfBoundsException(pos);  // TODO: Log this case
    }

    return costs[pos.y * width + pos.x];
}

std::vector<Position2D> SquareGridMap::neighbours(const Position2D& pos) const
{
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

void SquareGridMap::setCost(const Position2D& pos, float cost)
{
    if (isOutOfBounds(pos))
    {
        __lz::throwOutOfBoundsException(pos);  // TODO: Log this case
    }

    costs[pos.y * width + pos.x] = cost;
}

void SquareGridMap::setWalkable(const Position2D& pos, bool walkable)
{
    if (isWalkable(pos) && walkable)
        return;  // do nothing
    setCost(pos, walkable ? 1. : -1.);
}

void SquareGridMap::setTransparency(const Position2D& pos, bool transparent)
{
    if (isOutOfBounds(pos))
    {
        __lz::throwOutOfBoundsException(pos);  // TODO: Log this case
    }

    transparencies[pos.y * width + pos.x] = transparent;
}

void SquareGridMap::carveRoom(const Position2D& topLeft,
                              const Position2D& bottomRight,
                              float cost)
{
    for (int x = topLeft.x; x < bottomRight.x; ++x)
    {
        for (int y = topLeft.y; y < bottomRight.y; ++y)
        {
            Position2D pos(x, y);
            setCost(pos, cost);
            setTransparency(pos, true);
        }
    }
}