#include "catch/catch.hpp"

#include <lazarus/ECS/Entity.h>
#include <lazarus/common.h>

using namespace lz;

// Component for testing purposes
struct TestComponent
{
    TestComponent(int num)
        : num(num)
    {}

    int num;
};

// Another test component, with multiple constructor params
struct SecondTestComponent
{
    SecondTestComponent(std::string string, int num, bool testBool)
        : string(string)
        , num(num)
        , testBool(testBool)
    {}

    std::string string;
    int num;
    bool testBool;
};

// A test component with no data
struct EmptyComponent
{
};

TEST_CASE("entity creation")
{
    Entity entity;
    Identifier id = entity.getId();
    SECTION("entity ids")
    {
Entity another;
        REQUIRE(entity.getId() == id);  // The original entity's ID hasn't changed
        REQUIRE(another.getId() == id + 1);  // New entity gets new ID
    }
    SECTION("entity has no components on creation")
    {
        REQUIRE_FALSE(entity.has<TestComponent>());
        REQUIRE_FALSE(entity.has<SecondTestComponent>());
    }
    SECTION("entity not deleted on creation")
    {
        REQUIRE_FALSE(entity.isDeleted());
    }
}

TEST_CASE("adding components")
{
    Entity entity;
    SECTION("add one component with a single param to entity")
    {
        REQUIRE_NOTHROW(entity.addComponent<TestComponent>(25));
    }
    SECTION("add one component with multiple params to entity")
    {
        REQUIRE_NOTHROW(entity.addComponent<SecondTestComponent>(
            "test",
            14,
            false
        ));
    }
    SECTION("add an empty component")
    {
        REQUIRE_NOTHROW(entity.addComponent<EmptyComponent>());
    }
    // Note: No need to test the case in which we try to add a component
    // that's not derived from BaseComponent, as that would not even compile
    // thanks to the static_assert in addComponent
}

TEST_CASE("checking component existence in entities")
{
    Entity entity;
    entity.addComponent<TestComponent>(25);
    entity.addComponent<SecondTestComponent>("test", 14, false);
    SECTION("check if entity has or not one component type")
    {
        REQUIRE(entity.has<TestComponent>());
        REQUIRE(entity.has<SecondTestComponent>());
        REQUIRE_FALSE(entity.has<EmptyComponent>());
    }
    SECTION("checking if entity has multiple components")
    {
        REQUIRE(entity.has<TestComponent, SecondTestComponent>());
        REQUIRE_FALSE(entity.has<TestComponent, EmptyComponent>());
    }
}

TEST_CASE("getting components from entities")
{
    Entity entity;
    entity.addComponent<SecondTestComponent>("test", -5, false);
    SecondTestComponent *comp = entity.get<SecondTestComponent>();
    SECTION("getting existing component")
    {
        REQUIRE(comp != nullptr);
        REQUIRE(comp->num == -5);
        REQUIRE(comp->string == "test");
    }
    SECTION("modifying component from the handle")
    {
        comp->num = 100;
        SecondTestComponent *other = entity.get<SecondTestComponent>();
        REQUIRE(other->num == 100);
    }
    SECTION("getting non-existant component")
    {
        TestComponent *other = entity.get<TestComponent>();
        REQUIRE(other == nullptr);
    }
}

TEST_CASE("removing components from entities")
{
    Entity entity;
    entity.addComponent<TestComponent>(15);
    SECTION("removing existing component")
    {
        REQUIRE(entity.has<TestComponent>());
        REQUIRE_NOTHROW(entity.removeComponent<TestComponent>());
        REQUIRE_FALSE(entity.has<TestComponent>());
    }
    SECTION("removing non-existing component")
    {
        REQUIRE_THROWS_AS(entity.removeComponent<SecondTestComponent>(),
                          __lz::LazarusException);
    }
    SECTION("can add component of same type after removing")
    {
        REQUIRE_NOTHROW(entity.removeComponent<TestComponent>());
        REQUIRE_NOTHROW(entity.addComponent<TestComponent>(99));
        REQUIRE(entity.has<TestComponent>());
    }
}
