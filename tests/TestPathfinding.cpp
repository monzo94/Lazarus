#include "catch/catch.hpp"

#include <lazarus/AStarSearch.h>
#include <lazarus/SquareGridMap.h>

using namespace lz;

TEST_CASE("A* on grid map")
{
    SquareGridMap map(5, 5);
    SquareGridMap mapWithDiagonals(5, 5, true);

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

    SECTION("A* on grid map without diagonals and equal costs")
    {
        AStarSearch<Position2D, SquareGridMap> aStarSearch(map,
                                                           Position2D(1, 3),
                                                           Position2D(4, 3));
        REQUIRE_NOTHROW(aStarSearch.execute());
        REQUIRE(aStarSearch.getState() == SearchState::SUCCESS);
        auto path = aStarSearch.getPath();
        REQUIRE(path.size() == 5);
        REQUIRE(path[0] == Position2D(1, 4));
        REQUIRE(path[1] == Position2D(2, 4));
        REQUIRE(path[2] == Position2D(3, 4));
        REQUIRE(path[3] == Position2D(3, 3));
        REQUIRE(path[4] == Position2D(4, 3));
    }
    SECTION("A* on grid map with diagonals and equal costs")
    {
        AStarSearch<Position2D, SquareGridMap> aStarSearch(mapWithDiagonals,
                                                           Position2D(1, 3),
                                                           Position2D(4, 3));
        REQUIRE_NOTHROW(aStarSearch.execute());
        REQUIRE(aStarSearch.getState() == SearchState::SUCCESS);
        auto path = aStarSearch.getPath();
        REQUIRE(path.size() == 3);
        REQUIRE(path[0] == Position2D(2, 4));
        REQUIRE(path[2] == Position2D(4, 3));
    }
    SECTION("A* with non-existant path")
    {
        AStarSearch<Position2D, SquareGridMap> aStarSearch(map,
                                                           Position2D(4, 0),
                                                           Position2D(0, 0));
        REQUIRE_NOTHROW(aStarSearch.execute());
        REQUIRE(aStarSearch.getState() == SearchState::FAILED);
        REQUIRE_THROWS_AS(aStarSearch.getPath(), __lz::LazarusException);        
    }
    SECTION("getting path on an unfinished search")
    {
        AStarSearch<Position2D, SquareGridMap> aStarSearch(map,
                                                           Position2D(1, 3),
                                                           Position2D(4, 3));
        REQUIRE(aStarSearch.getState() == SearchState::READY);
        REQUIRE_THROWS_AS(aStarSearch.getPath(), __lz::LazarusException);
    }
    SECTION("re-initialize search")
    {
        AStarSearch<Position2D, SquareGridMap> aStarSearch(map,
                                                           Position2D(1, 3),
                                                           Position2D(4, 3));
        REQUIRE_NOTHROW(aStarSearch.execute());
        REQUIRE(aStarSearch.getState() == SearchState::SUCCESS);
        REQUIRE_NOTHROW(aStarSearch.init(Position2D(2, 3), Position2D(1, 2)));
        REQUIRE(aStarSearch.getState() == SearchState::READY);
        REQUIRE_THROWS_AS(aStarSearch.getPath(), __lz::LazarusException);
    }
    SECTION("A* on grid map with varying costs")
    {
        AStarSearch<Position2D, SquareGridMap> aStarSearch(mapWithDiagonals,
                                                           Position2D(3, 2),
                                                           Position2D(2, 4));
        mapWithDiagonals.setCost(3, 3, 10);
        REQUIRE_NOTHROW(aStarSearch.execute());
        // With cost 1, the shortest path has length 2
        // With a tile in the way with a big cost, the pathfinder takes a detour
        // making the length of the path equal 3
        auto path = aStarSearch.getPath();
        REQUIRE(path.size() == 3);
        REQUIRE(path[0] == Position2D(4, 3));
        REQUIRE(path[1] == Position2D(3, 4));
        REQUIRE(path[2] == Position2D(2, 4));
    }
}
