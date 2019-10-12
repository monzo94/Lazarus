#include "catch/catch.hpp"

#include <lazarus/Graphics.h>
#include <lazarus/common.h>

using namespace lz;

TEST_CASE("tileset loading")
{
    Tileset tileset;
    SECTION("uninitialized tileset")
    {
        REQUIRE_FALSE(tileset.is_loaded());
        REQUIRE(tileset.get_tile_width() == 0);
        REQUIRE(tileset.get_tile_height() == 0);
        REQUIRE(tileset.get_num_tiles() == 0);
        REQUIRE_THROWS_AS(tileset.get_tile(0),
                          __lz::LazarusException);
        REQUIRE_THROWS_AS(tileset.get_tile(0, 0),
                          __lz::LazarusException);
    }
    SECTION("loading correct tileset")
    {
        REQUIRE_NOTHROW(tileset.load("tests/dejavu_12x12.png"));
        REQUIRE(tileset.get_tile_width() == 12);
        REQUIRE(tileset.get_tile_height() == 12);
        REQUIRE(tileset.get_num_tiles() == 32 * 8);
    }
    SECTION("loading incorrect tileset")
    {
        // Non existent tileset
        REQUIRE_THROWS_AS(tileset.load("tests/i_dont_exist.png"),
                          __lz::LazarusException);
        // Tileset with incorrect name
        REQUIRE_THROWS_AS(tileset.load("tests/dejavu12x12.png"),
                          __lz::LazarusException);
        // Tileset with incorrect dimensions
        REQUIRE_THROWS_AS(tileset.load("tests/incorrect_12x12.png"),
                          __lz::LazarusException);
    }
}
