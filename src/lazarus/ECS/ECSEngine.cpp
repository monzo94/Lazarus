#include <lazarus/ECS/ECSEngine.h>

using namespace lz;

Entity* ECSEngine::add_entity()
{
    Entity entity;
    std::shared_ptr<Entity> ent_ptr = std::make_shared<Entity>(entity);
    entities[entity.get_id()] = ent_ptr;
    return ent_ptr.get();
}

void ECSEngine::add_entity(Entity& entity)
{
    // TODO: Log the case when entity already exists in the map
    entities[entity.get_id()] = std::make_shared<Entity>(entity);
}

Entity* ECSEngine::get_entity(Identifier entity_id)
{
    auto found = entities.find(entity_id);
    if (found == entities.end())
        return nullptr;
    return found->second.get();
}

void ECSEngine::update()
{
    // Update all updateable systems
    for (auto updateable : updateables)
    {
        updateable->update(*this);
    }

    // Run garbage collector
    garbage_collect();
}

void ECSEngine::garbage_collect()
{
    auto it = entities.begin();
    while (it != entities.end())
    {
        Entity* entity = it->second.get();
        if (entity->is_deleted())
            it = entities.erase(it);
        else
            ++it;
    }
}
