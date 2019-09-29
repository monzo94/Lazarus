#include "catch/catch.hpp"

#include <lazarus/Heuristics.h>

using namespace Catch::literals;

using namespace lz;

TEST_CASE("Manhattan distance")
{
    REQUIRE(manhattan_distance(Position2D(0, 0), Position2D(2, 1)) == 3.0_a);
    REQUIRE(manhattan_distance(Position2D(0, 0), Position2D(1, 1)) == 2.0_a);
    REQUIRE(manhattan_distance(Position2D(-1, 2), Position2D(-1, 2)) == 0.0_a);
    REQUIRE(manhattan_distance(Position2D(2, -5), Position2D(-5, 2)) == 14.0_a);
    REQUIRE(manhattan_distance(Position2D(-1, -1), Position2D(1, -1)) == 2.0_a);
    REQUIRE(manhattan_distance(Position2D(-1, -1), Position2D(-1, 1)) == 2.0_a);
}

TEST_CASE("Euclidean distance")
{
    REQUIRE(euclidean_distance(Position2D(0, 0), Position2D(2, 1)) == Approx(std::sqrt(5)));
    REQUIRE(euclidean_distance(Position2D(0, 0), Position2D(1, 1)) == Approx(std::sqrt(2)));
    REQUIRE(euclidean_distance(Position2D(-1, 2), Position2D(-1, 2)) == 0.0_a);
    REQUIRE(euclidean_distance(Position2D(2, -5), Position2D(-5, 2)) == Approx(7 * std::sqrt(2)));
    REQUIRE(euclidean_distance(Position2D(-1, -1), Position2D(1, -1)) == 2.0_a);
    REQUIRE(euclidean_distance(Position2D(-1, -1), Position2D(-1, 1)) == 2.0_a);
}

TEST_CASE("Chebyshev distance")
{
    REQUIRE(chebyshev_distance(Position2D(0, 0), Position2D(2, 1)) == 2.0_a);
    REQUIRE(chebyshev_distance(Position2D(0, 0), Position2D(1, 1)) == 1.0_a);
    REQUIRE(chebyshev_distance(Position2D(-1, 2), Position2D(-1, 2)) == 0.0_a);
    REQUIRE(chebyshev_distance(Position2D(2, -5), Position2D(-5, 2)) == 7.0_a);
    REQUIRE(chebyshev_distance(Position2D(-1, -1), Position2D(1, -1)) == 2.0_a);
    REQUIRE(chebyshev_distance(Position2D(-1, -1), Position2D(-1, 1)) == 2.0_a);
}

TEST_CASE("Octile distance")
{
    REQUIRE(octile_distance(Position2D(0, 0), Position2D(2, 1)) == Approx(1 + std::sqrt(2)));
    REQUIRE(octile_distance(Position2D(0, 0), Position2D(1, 1)) == Approx(std::sqrt(2)));
    REQUIRE(octile_distance(Position2D(-1, 2), Position2D(-1, 2)) == 0.0_a);
    REQUIRE(octile_distance(Position2D(2, -5), Position2D(-5, 2)) == Approx(7 * std::sqrt(2)));
    REQUIRE(octile_distance(Position2D(-1, -1), Position2D(1, -1)) == 2.0_a);
    REQUIRE(octile_distance(Position2D(-1, -1), Position2D(-1, 1)) == 2.0_a);
}