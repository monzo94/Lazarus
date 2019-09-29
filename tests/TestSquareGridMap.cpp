#include "catch/catch.hpp"

#include <lazarus/SquareGridMap.h>
#include <lazarus/common.h>

using namespace Catch::literals;

using namespace lz;

bool position_in_vector(const std::vector<Position2D>& vec, const Position2D& pos)
{
    for (auto pos_in_vec : vec)
        if (pos.x == pos_in_vec.x && pos.y == pos_in_vec.y)
            return true;
    return false;
}

TEST_CASE("map dimensions")
{
    SECTION("good dimensions")
    {
        SquareGridMap map(10, 15);
        REQUIRE(map.get_width() == 10);
        REQUIRE(map.get_height() == 15);
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
                REQUIRE_FALSE(map.is_out_of_bounds(Position2D(x, y)));
    }
    SECTION("unwalkable by default")
    {
        for (int x = 0; x < width; ++x)
            for (int y = 0; y < height; ++y)
                REQUIRE_FALSE(map.is_walkable(Position2D(x, y)));
    }
    SECTION("not transparent by default")
    {
        for (int x = 0; x < width; ++x)
            for (int y = 0; y < height; ++y)
                REQUIRE_FALSE(map.is_transparent(Position2D(x, y)));
    }
}

TEST_CASE("map operations")
{
    const int width{3};
    const int height{3};
    SquareGridMap map(width, height);
    std::vector<Position2D> out_of_bounds_positions{
        Position2D(5, 5), Position2D(-1, 2), Position2D(0, -2),
        Position2D(0, 3), Position2D(3, 1), Position2D(3, 3)
    };
    Position2D tile(1, 2);
    SECTION("out of bounds positions")
    {
        for (auto pos : out_of_bounds_positions)
        {
            REQUIRE(map.is_out_of_bounds(pos));
            REQUIRE(map.is_out_of_bounds(pos.x, pos.y));
        }
    }
    SECTION("walkability of out of bounds tile")
    {
        for (auto pos : out_of_bounds_positions)
        {
            REQUIRE_FALSE(map.is_walkable(pos));
            REQUIRE_FALSE(map.is_walkable(pos.y, pos.y));
        }
    }
    SECTION("transparency of out of bounds tile")
    {
        for (auto pos : out_of_bounds_positions)
        {
            REQUIRE_FALSE(map.is_transparent(pos));
            REQUIRE_FALSE(map.is_transparent(pos.x, pos.y));
        }
    }
    SECTION("getting cost of out of bounds tile throws error")
    {
        for (auto pos : out_of_bounds_positions)
        {
            REQUIRE_THROWS_AS(map.get_cost(pos), __lz::LazarusException);
            REQUIRE_THROWS_AS(map.get_cost(pos.y, pos.y), __lz::LazarusException);
        }
    }
    SECTION("setting cost of out of bounds tile throws error")
    {
        for (auto pos : out_of_bounds_positions)
        {
            REQUIRE_THROWS_AS(map.set_cost(pos, 5), __lz::LazarusException);
            REQUIRE_THROWS_AS(map.set_cost(pos.x, pos.y, 5), __lz::LazarusException);
        }
    }
    SECTION("setting transparency of out of bounds tile throws error")
    {
        for (auto pos : out_of_bounds_positions)
        {
            REQUIRE_THROWS_AS(map.set_transparency(pos, true), __lz::LazarusException);
            REQUIRE_THROWS_AS(map.set_transparency(pos.x, pos.y, true), __lz::LazarusException);
        }
    }
    SECTION("setting walkability of out of bounds tile throws error")
    {
        for (auto pos : out_of_bounds_positions)
        {
            REQUIRE_THROWS_AS(map.set_walkable(pos, true), __lz::LazarusException);
            REQUIRE_THROWS_AS(map.set_walkable(pos.x, pos.y, true), __lz::LazarusException);
        }
    }
    SECTION("modifying the cost of a tile")
    {
        REQUIRE_NOTHROW(map.set_cost(tile, 5));
        REQUIRE(map.get_cost(tile) == 5.0_a);
        REQUIRE_NOTHROW(map.set_cost(tile.x, tile.y, 2));
        REQUIRE(map.get_cost(tile.x, tile.y) == 2.0_a);
    }
    SECTION("modifying the walkability of a tile")
    {
        REQUIRE_FALSE(map.is_walkable(tile));
        REQUIRE_NOTHROW(map.set_walkable(tile, true));
        REQUIRE(map.is_walkable(tile));
        REQUIRE_NOTHROW(map.set_walkable(tile.x, tile.y, false));
        REQUIRE_FALSE(map.is_walkable(tile.x, tile.y));
    }
    SECTION("setting positive cost makes tile walkable")
    {
        REQUIRE_FALSE(map.is_walkable(tile));
        REQUIRE_NOTHROW(map.set_cost(tile, 5));
        REQUIRE(map.is_walkable(tile));
    }
    SECTION("setting negative cost makes tile unwalkable")
    {
        map.set_cost(tile, -2);
        REQUIRE_FALSE(map.is_walkable(tile));
    }
    SECTION("modifying the transparency of a tile")
    {
        REQUIRE_FALSE(map.is_transparent(tile));
        REQUIRE_NOTHROW(map.set_transparency(tile, true));
        REQUIRE(map.is_transparent(tile));
        REQUIRE_NOTHROW(map.set_transparency(tile.x, tile.y, false));
        REQUIRE_FALSE(map.is_transparent(tile));
    }
    SECTION("getting the cost of an unwalkable tile throws an error")
    {
        REQUIRE_FALSE(map.is_walkable(tile));
        REQUIRE_THROWS_AS(map.get_cost(tile), __lz::LazarusException);
    }
}

TEST_CASE("neighbours")
{
    const int width{5};
    const int height{5};
    SquareGridMap map(width, height);
    SquareGridMap map_with_diagonals(width, height, true);

    // Make the following hard-coded map:
    // ...#.
    // .#..#
    // ###.#
    // ..#..
    // ....#
    std::vector<Position2D> walkable_tiles{
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
    for (auto pos : walkable_tiles)
    {
        map.set_walkable(pos, true);
        map_with_diagonals.set_walkable(pos, true);
    }
    SECTION("neighbours of walkable corner/border tiles for map without diagonals")
    {
        // Top-left corner
        auto neighbours = map.neighbours(Position2D(0, 0));
        REQUIRE(neighbours.size() == 2);
        REQUIRE(position_in_vector(neighbours, Position2D(1, 0)));
        REQUIRE(position_in_vector(neighbours, Position2D(0, 1)));

        // Top-right corner
        neighbours = map.neighbours(Position2D(4, 0));
        REQUIRE(neighbours.empty());

        // Right border
        neighbours = map.neighbours(Position2D(4, 3));
        REQUIRE(neighbours.size() == 1);
        REQUIRE(position_in_vector(neighbours, Position2D(3, 3)));
    }
    SECTION("neighbours of walkable non-corner/border tiles for map without diagonals")
    {
        auto neighbours = map.neighbours(Position2D(1, 3));
        REQUIRE(neighbours.size() == 2);
        REQUIRE(position_in_vector(neighbours, Position2D(0, 3)));
        REQUIRE(position_in_vector(neighbours, Position2D(1, 4)));
    }
    SECTION("neighbours of out of bounds tile throws an exception")
    {
        // Out of bounds tile
        REQUIRE_THROWS_AS(map.neighbours(Position2D(10, 15)), __lz::LazarusException);
    }
    SECTION("neighbours of walkable corner/border tiles for map with diagonals")
    {
        // Top-left corner
        auto neighbours = map_with_diagonals.neighbours(Position2D(0, 0));
        REQUIRE(neighbours.size() == 2);
        REQUIRE(position_in_vector(neighbours, Position2D(1, 0)));
        REQUIRE(position_in_vector(neighbours, Position2D(0, 1)));

        // Top-right corner
        neighbours = map_with_diagonals.neighbours(Position2D(4, 0));
        REQUIRE(neighbours.size() == 1);
        REQUIRE(position_in_vector(neighbours, Position2D(3, 1)));

        // Right border
        neighbours = map_with_diagonals.neighbours(Position2D(4, 3));
        REQUIRE(neighbours.size() == 3);
        REQUIRE(position_in_vector(neighbours, Position2D(3, 2)));
        REQUIRE(position_in_vector(neighbours, Position2D(3, 3)));
        REQUIRE(position_in_vector(neighbours, Position2D(3, 4)));
    }
    SECTION("neighbours of walkable non-corner/border tiles for map with diagonals")
    {
        auto neighbours = map_with_diagonals.neighbours(Position2D(1, 3));
        REQUIRE(neighbours.size() == 4);
        REQUIRE(position_in_vector(neighbours, Position2D(0, 3)));
        REQUIRE(position_in_vector(neighbours, Position2D(1, 4)));
        REQUIRE(position_in_vector(neighbours, Position2D(0, 4)));
        REQUIRE(position_in_vector(neighbours, Position2D(2, 4)));
    }
    SECTION("neighbours with coordinates")
    {
        auto neighbours = map.neighbours(2, 1);
        REQUIRE(neighbours.size() == 2);
        REQUIRE(position_in_vector(neighbours, Position2D(3, 1)));
        REQUIRE(position_in_vector(neighbours, Position2D(2, 0)));
    }
}

TEST_CASE("carve_room")
{
    const int width{5};
    const int height{5};
    SquareGridMap map(width, height);
    SECTION("carve room in the center")
    {
        REQUIRE_NOTHROW(map.carve_room(Position2D(1, 1), Position2D(3, 3)));
        for (int x = 0; x < width; ++x)
        {
            for (int y = 0; y < height; ++y)
            {
                if (x >= 1 && x <= 3 && y >= 1 && y <= 3)
                {
                    REQUIRE(map.is_walkable(x, y));
                    REQUIRE(map.is_transparent(x, y));
                }
                else
                {
                    REQUIRE_FALSE(map.is_walkable(x, y));
                    REQUIRE_FALSE(map.is_transparent(x, y));
                }
            }
        }
    }
    SECTION("carve room outside of boundaries")
    {
        REQUIRE_THROWS_AS(map.carve_room(Position2D(3, 3), Position2D(7, 7)),
                          __lz::LazarusException);
    }
    SECTION("carve room with wrong corners")
    {
        REQUIRE_NOTHROW(map.carve_room(Position2D(3, 3), Position2D(1, 1)));
        for (int x = 0; x < width; ++x)
            for (int y = 0; y < height; ++y)
                REQUIRE_FALSE(map.is_walkable(x, y));  // No changes made
    }
}
