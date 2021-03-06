#include <lazarus/AStarSearch.h>
#include <lazarus/SquareGridMap.h>

#include "catch/catch.hpp"

using namespace lz;

TEST_CASE("A* on grid map")
{
    // Make the following hard-coded map:
    // ...#.
    // .#..#
    // ###.#
    // ..#..
    // ....#
    std::vector<std::vector<int>> prefab{
        {1, 1, 1, 0, 1},
        {1, 0, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {1, 1, 0, 1, 1},
        {1, 1, 1, 1, 0},
    };
    SquareGridMap map(prefab);
    SquareGridMap map_with_diagonals(prefab, true);

    SECTION("A* on grid map without diagonals and equal costs")
    {
        AStarSearch<Position2D, SquareGridMap> astar_search(
            map, Position2D(1, 3), Position2D(4, 3));
        REQUIRE_NOTHROW(astar_search.execute());
        REQUIRE(astar_search.get_state() == SearchState::SUCCESS);
        auto path = astar_search.getPath();
        REQUIRE(path.size() == 5);
        REQUIRE(path[0] == Position2D(1, 4));
        REQUIRE(path[1] == Position2D(2, 4));
        REQUIRE(path[2] == Position2D(3, 4));
        REQUIRE(path[3] == Position2D(3, 3));
        REQUIRE(path[4] == Position2D(4, 3));
    }
    SECTION("A* on grid map with diagonals and equal costs")
    {
        AStarSearch<Position2D, SquareGridMap> astar_search(
            map_with_diagonals, Position2D(1, 3), Position2D(4, 3));
        REQUIRE_NOTHROW(astar_search.execute());
        REQUIRE(astar_search.get_state() == SearchState::SUCCESS);
        auto path = astar_search.getPath();
        REQUIRE(path.size() == 3);
        REQUIRE(path[0] == Position2D(2, 4));
        REQUIRE(path[2] == Position2D(4, 3));
    }
    SECTION("A* with non-existant path")
    {
        AStarSearch<Position2D, SquareGridMap> astar_search(
            map, Position2D(4, 0), Position2D(0, 0));
        REQUIRE_NOTHROW(astar_search.execute());
        REQUIRE(astar_search.get_state() == SearchState::FAILED);
        REQUIRE_THROWS_AS(astar_search.getPath(), __lz::LazarusException);
    }
    SECTION("getting path on an unfinished search")
    {
        AStarSearch<Position2D, SquareGridMap> astar_search(
            map, Position2D(1, 3), Position2D(4, 3));
        REQUIRE(astar_search.get_state() == SearchState::READY);
        REQUIRE_THROWS_AS(astar_search.getPath(), __lz::LazarusException);
    }
    SECTION("re-initialize search")
    {
        AStarSearch<Position2D, SquareGridMap> astar_search(
            map, Position2D(1, 3), Position2D(4, 3));
        REQUIRE_NOTHROW(astar_search.execute());
        REQUIRE(astar_search.get_state() == SearchState::SUCCESS);
        REQUIRE_NOTHROW(astar_search.init(Position2D(2, 3), Position2D(1, 2)));
        REQUIRE(astar_search.get_state() == SearchState::READY);
        REQUIRE_THROWS_AS(astar_search.getPath(), __lz::LazarusException);
    }
    SECTION("A* on grid map with varying costs")
    {
        AStarSearch<Position2D, SquareGridMap> astar_search(
            map_with_diagonals, Position2D(3, 2), Position2D(2, 4));
        map_with_diagonals.set_cost(3, 3, 10);
        REQUIRE_NOTHROW(astar_search.execute());
        // With cost 1, the shortest path has length 2
        // With a tile in the way with a big cost, the pathfinder takes a detour
        // making the length of the path equal 3
        auto path = astar_search.getPath();
        REQUIRE(path.size() == 3);
        REQUIRE(path[0] == Position2D(4, 3));
        REQUIRE(path[1] == Position2D(3, 4));
        REQUIRE(path[2] == Position2D(2, 4));
    }
}
