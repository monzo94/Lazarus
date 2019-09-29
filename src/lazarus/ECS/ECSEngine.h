#pragma once

#include <functional>
#include <sstream>

#include <lazarus/ECS/Entity.h>
#include <lazarus/ECS/EventListener.h>
#include <lazarus/ECS/Updateable.h>

namespace lz
{
/**
 * Main driver to work with entities, components and systems.
 * 
 * The ECSEngine class manages and puts together all the pieces of the ECS model.
 * It holds the entities and systems, and acts as a controller for the creation,
 * deletion and modification of these objects.
 * 
 * It also acts as an observer for the classes that implement the EventListener
 * interface, and is able to emit events of any type to these listeners, allowing
 * communication between systems.
 * 
 * @see Entity
 * @see BaseSystem
 * @see EventListener
 */
class ECSEngine
{
public:
    /**
     * Adds a new entity to the collection and returns a pointer to it.
     */
    Entity* add_entity();

    /**
     * Adds an existing entity to the collection.
     * 
     * If the entity already exists in the collection, that is, the ID of
     * the entity is found in the collection, it does nothing.
     */
    void add_entity(Entity& entity);

    /**
     * Gets a pointer to the entity from the collection with the given
     * ID, or a nullptr if an entity with such ID does not exist in the
     * collection.
     */
    Entity* get_entity(Identifier entity_id);

    /**
     * Returns a vector with the entities that have the specified
     * components.
     * 
     * If include_deleted is set to true, entities that are marked for
     * deletion will also be included.
     */
    template <typename... Types>
    std::vector<Entity*> entities_with_components(bool include_deleted=false);

    /**
     * Applies a function to each of the entities from the collection that have the
     * specified component types.
     * 
     * The function passed can be a reference to an existing function, a lambda,
     * or an std::function.
     * 
     * If include_deleted is set to true, the function will also be applied to
     * entities that are marked for deletion.
     */
    template <typename... Types>
    void apply_to_each(
        typename std::common_type<std::function<void(Entity*, Types*...)>>::type&& func,
        bool include_deleted=false);

    /**
     * Subscribes the event listener to the list of listeners of that event type.
     * 
     * When an event of type EventType is emitted, it will be passed to all the
     * event listeners subscribed to this event type, by calling their receive
     * method.
     * 
     * @see EventListener
     */
    template <typename EventType>
    void subscribe(EventListener<EventType>* event_listener);

    /**
     * Unsubscribes the event listener from the list of listeners of that event type.
     */
    template <typename EventType>
    void unsubscribe(EventListener<EventType>* event_listener);

    /**
     * Emit an event to all listeners of that type of event.
     */
    template <typename EventType>
    void emit(const EventType& event);

    /**
     * Adds an updateable object to the engine.
     * 
     * The update method on this object will be called when the engine is updated.
     */
    void register_updateable(Updateable* updateable);

    /**
     * Updates all the updateable objects in the engine.
     * 
     * Will also garbage collect deleted entities.
     */
    virtual void update();

private:
    /**
     * Removes deleted entities.
     */
    void garbage_collect();

private:
    std::unordered_map<Identifier, std::shared_ptr<Entity>> entities;
    std::vector<Updateable*> updateables;
    // Maps event type index -> list of event listeners for that event type
    std::unordered_map<std::type_index,
                       std::vector<__lz::BaseEventListener*>> subscribers;
};

template <typename... Types>
std::vector<Entity*> ECSEngine::entities_with_components(bool include_deleted)
{
    std::vector<Entity*> result;
    apply_to_each<Types...>([&](Entity* ent, Types*... comp)
    {
        result.push_back(ent);
    },
    include_deleted);
    return result;
}

template <typename... Types>
void ECSEngine::apply_to_each(
    typename std::common_type<std::function<void(Entity*, Types*...)>>::type&& func,
    bool include_deleted)
{
    for (auto it = entities.begin(); it != entities.end(); ++it)
    {
        Entity* entity = it->second.get();

        if (!include_deleted && entity->is_deleted())
            continue;

        if (entity->has<Types...>())
            func(entity, entity->get<Types>()...);
    }
}

template <typename EventType>
void ECSEngine::subscribe(EventListener<EventType>* event_listener)
{
    std::type_index typeId = __lz::get_type_index<EventType>();
    auto found = subscribers.find(typeId);
    if (found == subscribers.end())
    {
        // No subscribers to this type of event yet, create vector
        std::vector<__lz::BaseEventListener*> vec;
        vec.push_back(event_listener);
        subscribers[typeId] = vec;
    }
    else
    {
        // There already exists a list of subscribers to this event type
        found->second.push_back(event_listener);
    }
}

template <typename EventType>
void ECSEngine::unsubscribe(EventListener<EventType>* event_listener)
{
    auto found = subscribers.find(__lz::get_type_index<EventType>());
    if (found != subscribers.end())
    {
        auto event_listeners = found->second;
        for (auto it = event_listeners.begin(); it != event_listeners.end(); ++it)
        {
            if (*it == event_listener)
            {
                // System found, remove it from the subscriber list
                event_listeners.erase(it);
                return;
            }
        }
    }
    // System was not found
    std::stringstream msg;
    msg << "ECS engine was not subscribed to the event ";
    msg << typeid(EventType).name();
    throw __lz::LazarusException(msg.str());
}

template <typename EventType>
void ECSEngine::emit(const EventType& event)
{
    // TODO: Log case in which an event is emitted but no listeners for that type exist
    auto found = subscribers.find(__lz::get_type_index<EventType>());
    if (found != subscribers.end())
    {
        auto event_listeners = found->second;
        for (auto it = event_listeners.begin(); it != event_listeners.end(); ++it)
        {
            auto* listener = dynamic_cast<EventListener<EventType>*>(*it);
            listener->receive(*this, event);
        }
    }
}
}  // namespace lz
