#include <lazarus/ECS/Entity.h>

using namespace lz;

Identifier Entity::entity_count = 0;

Entity::Entity()
    : entity_id(++entity_count)
{
}

bool Entity::operator==(const Entity &other)
{
    return get_id() == other.get_id();
}

bool Entity::operator!=(const Entity &other)
{
    return !(*this == other);
}

bool Entity::operator<(const Entity &other)
{
    return get_id() < other.get_id();
}
