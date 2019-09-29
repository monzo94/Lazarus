#include "catch/catch.hpp"

#include <cmath>
#include <typeinfo>
#include <vector>

#include <lazarus/Random.h>

#define TEST_SEED 12345

using namespace Catch::literals;

using namespace lz;

TEST_CASE("int range generation", "[random]")
{
    Random::seed(TEST_SEED);
    SECTION("generation with fixed interval")
    { // The first 10 numbers generated with range(0, 3)
        // with the given seed should coincide with these
        std::vector<int> sequence{3, 3, 1, 0, 0, 0, 0, 3, 2, 2};
        for (int i = 0; i < sequence.size(); ++i)
            REQUIRE(Random::range(0, 3) == sequence[i]);
    }
    SECTION("varying intervals")
    {
        REQUIRE(Random::range(0, 2) == 2);
        REQUIRE(Random::range(5, 10) == 10);
        REQUIRE(Random::range(-1, 1) == -1);
        REQUIRE(Random::range(0, 0) == 0);
        REQUIRE(Random::range(2, -5) == -4);
    }
}

TEST_CASE("one_in test", "[random]")
{
    Random::seed(TEST_SEED);
    SECTION("sequence with same probability")
    { // With this seed, only the 6th roll of one_in(10) returns true
        for (int i = 0; i < 10; ++i)
        {
            if (i == 5)
                REQUIRE(Random::one_in(10));
            else
                REQUIRE(!Random::one_in(10));
        }
    }
    SECTION("different probabilities")
    {
        REQUIRE(!Random::one_in(5));
        // -1 gets casted to a huge integer since the parameter is unsigned and should
        // therefore be false almost always
        REQUIRE(!Random::one_in(-1));
        // one_in(0) is true by definition
        REQUIRE(Random::one_in(0));
    }
    SECTION("one_in(1) is always true")
    {
        // one_in(1) should be true always
        for (int i = 0; i < 15; ++i)
            REQUIRE(Random::one_in(1) == 1);
    }
}

TEST_CASE("roll test", "[random]")
{
    Random::seed(TEST_SEED);
    REQUIRE(Random::roll(6, 2) == 12);
    REQUIRE(Random::roll(2, 5) == 5);
    REQUIRE(Random::roll(20, 1) == 17);
    // Test base cases
    REQUIRE(Random::roll(0, 0) == 0);
    REQUIRE(Random::roll(1, 0) == 0);
    REQUIRE(Random::roll(0, 5) == 0);
}

TEST_CASE("normal distribution", "[random]")
{
    Random::seed(TEST_SEED);
    REQUIRE(Random::normal(0, 2) == 1.1901823323_a);
    REQUIRE(Random::normal(5, 10) == 13.00446708_a);
    REQUIRE(Random::normal(-1, 0.5) == -1.52153444_a);
    REQUIRE(Random::normal(0, 0) == 0.0_a);
    REQUIRE(Random::normal(0, -1) == -0.488176471_a);
}

TEST_CASE("float range", "[random]")
{
    Random::seed(TEST_SEED);
    REQUIRE(Random::range(0., 2) == 1.7803094265_a);
    REQUIRE(Random::range(0, 2.) == 0.26141458811_a);
    REQUIRE(Random::range(-0.5, 0.5) == -0.460240503_a);
    REQUIRE(Random::range(0., 0.) == 0.0_a);
    REQUIRE(Random::range(1e-5, 2e-5) == 1.5320779165e-5_a);

}

TEST_CASE("range types", "[random]")
{
    REQUIRE(typeid(Random::range(static_cast<int>(0), static_cast<int>(2))) == typeid(int));
    REQUIRE(typeid(Random::range(static_cast<int>(0), static_cast<short>(2))) == typeid(int));
    REQUIRE(typeid(Random::range(static_cast<short>(0), static_cast<short>(2))) == typeid(short));
    REQUIRE(typeid(Random::range(static_cast<long>(0), static_cast<unsigned long long>(2))) == typeid(unsigned long long));
    REQUIRE(typeid(Random::range(static_cast<int>(0), static_cast<float>(2))) == typeid(float));
    REQUIRE(typeid(Random::range(static_cast<float>(0), static_cast<long>(2))) == typeid(float));
    REQUIRE(typeid(Random::range(static_cast<double>(0), static_cast<float>(2))) == typeid(double));
    REQUIRE(typeid(Random::range(static_cast<long>(0), static_cast<int>(2))) == typeid(long));
    REQUIRE(typeid(Random::range(static_cast<char>(0), static_cast<double>(2))) == typeid(double));
}
