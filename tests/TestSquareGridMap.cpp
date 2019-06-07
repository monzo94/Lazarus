#include "catch/catch.hpp"

#include <lazarus/SquareGridMap.h>
#include <lazarus/common.h>

using namespace Catch::literals;

using namespace lz;

bool positionInVector(const std::vector<Position2D>& vec, const Position2D& pos)
{
    for (auto posInVec : vec)
        if (pos.x == posInVec.x && pos.y == posInVec.y)
            return true;
    return false;
}

TEST_CASE("map dimensions")
{
    SECTION("good dimensions")
    {
        SquareGridMap map(10, 15);
        REQUIRE(map.getWidth() == 10);
        REQUIRE(map.getHeight() == 15);
    }
    SECTION("bad dimensions")
    {
        // Setting width or height to 0 should throw an exception
        REQUIRE_THROWS_AS(SquareGridMap(0, 5),
                          __lz::LazarusException);
        REQUIRE_THROWS_AS(SquareGridMap(5, 0),
                          __lz::LazarusException);
    }
}

TEST_CASE("map defaults")
{
    const int width{3};
    const int height{3};
    SquareGridMap map(width, height);
    SECTION("all tiles within bounds")
    {
        for (int x = 0; x < width; ++x)
            for (int y = 0; y < height; ++y)
                REQUIRE_FALSE(map.isOutOfBounds(Position2D(x, y)));
    }
    SECTION("unwalkable by default")
    {
        for (int x = 0; x < width; ++x)
            for (int y = 0; y < height; ++y)
                REQUIRE_FALSE(map.isWalkable(Position2D(x, y)));
    }
    SECTION("not transparent by default")
    {
        for (int x = 0; x < width; ++x)
            for (int y = 0; y < height; ++y)
                REQUIRE_FALSE(map.isTransparent(Position2D(x, y)));
    }
}

TEST_CASE("map operations")
{
    const int width{3};
    const int height{3};
    SquareGridMap map(width, height);
    std::vector<Position2D> outOfBoundsPositions{
        Position2D(5, 5), Position2D(-1, 2), Position2D(0, -2),
        Position2D(0, 3), Position2D(3, 1), Position2D(3, 3)
    };
    Position2D tile(1, 2);
    SECTION("out of bounds positions")
    {
        for (auto pos : outOfBoundsPositions)
        {
            REQUIRE(map.isOutOfBounds(pos));
            REQUIRE(map.isOutOfBounds(pos.x, pos.y));
        }
    }
    SECTION("walkability of out of bounds tile")
    {
        for (auto pos : outOfBoundsPositions)
        {
            REQUIRE_FALSE(map.isWalkable(pos));
            REQUIRE_FALSE(map.isWalkable(pos.y, pos.y));
        }
    }
    SECTION("transparency of out of bounds tile")
    {
        for (auto pos : outOfBoundsPositions)
        {
            REQUIRE_FALSE(map.isTransparent(pos));
            REQUIRE_FALSE(map.isTransparent(pos.x, pos.y));
        }
    }
    SECTION("getting cost of out of bounds tile throws error")
    {
        for (auto pos : outOfBoundsPositions)
        {
            REQUIRE_THROWS_AS(map.getCost(pos), __lz::LazarusException);
            REQUIRE_THROWS_AS(map.getCost(pos.y, pos.y), __lz::LazarusException);
        }
    }
    SECTION("setting cost of out of bounds tile throws error")
    {
        for (auto pos : outOfBoundsPositions)
        {
            REQUIRE_THROWS_AS(map.setCost(pos, 5), __lz::LazarusException);
            REQUIRE_THROWS_AS(map.setCost(pos.x, pos.y, 5), __lz::LazarusException);
        }
    }
    SECTION("setting transparency of out of bounds tile throws error")
    {
        for (auto pos : outOfBoundsPositions)
        {
            REQUIRE_THROWS_AS(map.setTransparency(pos, true), __lz::LazarusException);
            REQUIRE_THROWS_AS(map.setTransparency(pos.x, pos.y, true), __lz::LazarusException);
        }
    }
    SECTION("setting walkability of out of bounds tile throws error")
    {
        for (auto pos : outOfBoundsPositions)
        {
            REQUIRE_THROWS_AS(map.setWalkable(pos, true), __lz::LazarusException);
            REQUIRE_THROWS_AS(map.setWalkable(pos.x, pos.y, true), __lz::LazarusException);
        }
    }
    SECTION("modifying the cost of a tile")
    {
        REQUIRE_NOTHROW(map.setCost(tile, 5));
        REQUIRE(map.getCost(tile) == 5.0_a);
        REQUIRE_NOTHROW(map.setCost(tile.x, tile.y, 2));
        REQUIRE(map.getCost(tile.x, tile.y) == 2.0_a);
    }
    SECTION("modifying the walkability of a tile")
    {
        REQUIRE_FALSE(map.isWalkable(tile));
        REQUIRE_NOTHROW(map.setWalkable(tile, true));
        REQUIRE(map.isWalkable(tile));
        REQUIRE_NOTHROW(map.setWalkable(tile.x, tile.y, false));
        REQUIRE_FALSE(map.isWalkable(tile.x, tile.y));
    }
    SECTION("setting positive cost makes tile walkable")
    {
        REQUIRE_FALSE(map.isWalkable(tile));
        REQUIRE_NOTHROW(map.setCost(tile, 5));
        REQUIRE(map.isWalkable(tile));
    }
    SECTION("setting negative cost makes tile unwalkable")
    {
        map.setCost(tile, -2);
        REQUIRE_FALSE(map.isWalkable(tile));
    }
    SECTION("modifying the transparency of a tile")
    {
        REQUIRE_FALSE(map.isTransparent(tile));
        REQUIRE_NOTHROW(map.setTransparency(tile, true));
        REQUIRE(map.isTransparent(tile));
        REQUIRE_NOTHROW(map.setTransparency(tile.x, tile.y, false));
        REQUIRE_FALSE(map.isTransparent(tile));
    }
    SECTION("getting the cost of an unwalkable tile throws an error")
    {
        REQUIRE_FALSE(map.isWalkable(tile));
        REQUIRE_THROWS_AS(map.getCost(tile), __lz::LazarusException);
    }
}

TEST_CASE("neighbours")
{
    const int width{5};
    const int height{5};
    SquareGridMap map(width, height);
    SquareGridMap mapWithDiagonals(width, height, true);

    // Make the following hard-coded map:
    // ...#.
    // .#..#
    // ###.#
    // ..#..
    // ....#
    std::vector<Position2D> walkableTiles{
        Position2D(0, 0),
        Position2D(1, 0),
        Position2D(2, 0),
        Position2D(0, 0),
        Position2D(4, 0),
        Position2D(0, 1),
        Position2D(2, 1),
        Position2D(3, 1),
        Position2D(3, 2),
        Position2D(0, 3),
        Position2D(1, 3),
        Position2D(3, 3),
        Position2D(4, 3),
        Position2D(0, 4),
        Position2D(1, 4),
        Position2D(2, 4),
        Position2D(3, 4)
    };
    for (auto pos : walkableTiles)
    {
        map.setWalkable(pos, true);
        mapWithDiagonals.setWalkable(pos, true);
    }
    SECTION("neighbours of walkable corner/border tiles for map without diagonals")
    {
        // Top-left corner
        auto neighbours = map.neighbours(Position2D(0, 0));
        REQUIRE(neighbours.size() == 2);
        REQUIRE(positionInVector(neighbours, Position2D(1, 0)));
        REQUIRE(positionInVector(neighbours, Position2D(0, 1)));

        // Top-right corner
        neighbours = map.neighbours(Position2D(4, 0));
        REQUIRE(neighbours.empty());

        // Right border
        neighbours = map.neighbours(Position2D(4, 3));
        REQUIRE(neighbours.size() == 1);
        REQUIRE(positionInVector(neighbours, Position2D(3, 3)));
    }
    SECTION("neighbours of walkable non-corner/border tiles for map without diagonals")
    {
        auto neighbours = map.neighbours(Position2D(1, 3));
        REQUIRE(neighbours.size() == 2);
        REQUIRE(positionInVector(neighbours, Position2D(0, 3)));
        REQUIRE(positionInVector(neighbours, Position2D(1, 4)));
    }
    SECTION("neighbours of out of bounds tile throws an exception")
    {
        // Out of bounds tile
        REQUIRE_THROWS_AS(map.neighbours(Position2D(10, 15)), __lz::LazarusException);
    }
    SECTION("neighbours of walkable corner/border tiles for map with diagonals")
    {
        // Top-left corner
        auto neighbours = mapWithDiagonals.neighbours(Position2D(0, 0));
        REQUIRE(neighbours.size() == 2);
        REQUIRE(positionInVector(neighbours, Position2D(1, 0)));
        REQUIRE(positionInVector(neighbours, Position2D(0, 1)));

        // Top-right corner
        neighbours = mapWithDiagonals.neighbours(Position2D(4, 0));
        REQUIRE(neighbours.size() == 1);
        REQUIRE(positionInVector(neighbours, Position2D(3, 1)));

        // Right border
        neighbours = mapWithDiagonals.neighbours(Position2D(4, 3));
        REQUIRE(neighbours.size() == 3);
        REQUIRE(positionInVector(neighbours, Position2D(3, 2)));
        REQUIRE(positionInVector(neighbours, Position2D(3, 3)));
        REQUIRE(positionInVector(neighbours, Position2D(3, 4)));
    }
    SECTION("neighbours of walkable non-corner/border tiles for map with diagonals")
    {
        auto neighbours = mapWithDiagonals.neighbours(Position2D(1, 3));
        REQUIRE(neighbours.size() == 4);
        REQUIRE(positionInVector(neighbours, Position2D(0, 3)));
        REQUIRE(positionInVector(neighbours, Position2D(1, 4)));
        REQUIRE(positionInVector(neighbours, Position2D(0, 4)));
        REQUIRE(positionInVector(neighbours, Position2D(2, 4)));
    }
    SECTION("neighbours with coordinates")
    {
        auto neighbours = map.neighbours(2, 1);
        REQUIRE(neighbours.size() == 2);
        REQUIRE(positionInVector(neighbours, Position2D(3, 1)));
        REQUIRE(positionInVector(neighbours, Position2D(2, 0)));
    }
}

TEST_CASE("carveRoom")
{
    const int width{5};
    const int height{5};
    SquareGridMap map(width, height);
    SECTION("carve room in the center")
    {
        REQUIRE_NOTHROW(map.carveRoom(Position2D(1, 1), Position2D(3, 3)));
        for (int x = 0; x < width; ++x)
        {
            for (int y = 0; y < height; ++y)
            {
                if (x >= 1 && x <= 3 && y >= 1 && y <= 3)
                {
                    REQUIRE(map.isWalkable(x, y));
                    REQUIRE(map.isTransparent(x, y));
                }
                else
                {
                    REQUIRE_FALSE(map.isWalkable(x, y));
                    REQUIRE_FALSE(map.isTransparent(x, y));
                }
            }
        }
    }
    SECTION("carve room outside of boundaries")
    {
        REQUIRE_THROWS_AS(map.carveRoom(Position2D(3, 3), Position2D(7, 7)),
                          __lz::LazarusException);
    }
    SECTION("carve room with wrong corners")
    {
        REQUIRE_NOTHROW(map.carveRoom(Position2D(3, 3), Position2D(1, 1)));
        for (int x = 0; x < width; ++x)
            for (int y = 0; y < height; ++y)
                REQUIRE_FALSE(map.isWalkable(x, y));  // No changes made
    }
}
