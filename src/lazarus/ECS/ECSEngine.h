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
     * Registers a system to listen to certain event types.
     * 
     * The system must subclass an event listener of each of the event
     * types passed, and therefore must implement each receive method
     * for each of the event types.
     * 
     * When an event of type EventType is emitted, it will be passed to all the
     * event listeners subscribed to this event type, by calling their receive
     * method.
     * 
     * If a system of the specified type already exists in the engine, it
     * will be set to also listen to the new event types.
     * 
     * @see EventListener
     */
    template <typename System, typename... EventTypes>
    void add_system();

    /**
     * Deletes a system from the engine, making it stop receiveng any events
     * of the event type it was listening.
     */
    template <typename System>
    void delete_system();

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

    /**
     * Checks that the system is a listener of the event type.
     */
    template <typename System, typename EventType>
    static bool verify_listener();

    /**
     * Checks that the system is a listener of all the event types.
     */
    template <typename System, typename T, typename V,typename... Types>
    static bool verify_listener();

    /**
     * Searches for the system in the list of subscribers to events,
     * and returns a pointer to it if it is found, or a nullptr if it
     * does not exist in the engine.
     */
    template <typename System>
    std::shared_ptr<System> find_system();

    /**
     * Checks if a system of the given type is already listening
     * to a certain event type.
     */
    template <typename System, typename EventType>
    bool is_listener() const;

    /**
     * Checks if a system of the given type id is already listening
     * to a certain event type.
     */
    template <typename System>
    bool is_listener(std::type_index type_id) const;

private:
    std::unordered_map<Identifier, std::shared_ptr<Entity>> entities;
    std::vector<Updateable*> updateables;
    // Maps event type index -> list of event listeners for that event type
    std::unordered_map<std::type_index,
                       std::vector<std::shared_ptr<__lz::BaseEventListener>>> subscribers;
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

template <typename System, typename... EventTypes>
void ECSEngine::add_system()
{
    if (!verify_listener<System, EventTypes...>())
        throw __lz::LazarusException("System is not a listener to some of the event types");

    // Put all event types in a vector
    std::vector<std::type_index> types{__lz::get_type_index<EventTypes>()...};

    // Search for the system, and if it exists, use it
    std::shared_ptr<System> new_system = find_system<System>();
    // If the system does not exist, create a new one
    if (!new_system)
        new_system = std::make_shared<System>();

    for (std::type_index type_id : types)
    {
        auto found = subscribers.find(type_id);
        if (found == subscribers.end())
        {
            // No subscribers to this type of event yet, create vector
            std::vector<std::shared_ptr<__lz::BaseEventListener>> vec;
            vec.push_back(new_system);
            subscribers[type_id] = vec;
        }
        else
        {
            // There already exists a list of subscribers to this event type
            if (!is_listener<System>(type_id))
                found->second.push_back(new_system);
        }
    }
}

template <typename System>
void ECSEngine::delete_system()
{
    // Remove the system from all the event listeners
    for (auto &pair : subscribers)
    {
        auto &systems = pair.second;
        for (auto it = systems.begin(); it != systems.end(); ++it)
        {
            if (dynamic_cast<System*>(it->get()))
            {
                systems.erase(it);
                break;
            }
        }
    }
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
            auto* listener = dynamic_cast<EventListener<EventType>*>(it->get());
            listener->receive(*this, event);
        }
    }
}

template <typename System, typename EventType>
bool ECSEngine::verify_listener()
{
    return std::is_base_of<EventListener<EventType>, System>::value;
}

template <typename System, typename T, typename V,typename... Types>
bool ECSEngine::verify_listener()
{
    return verify_listener<System, T>() && verify_listener<System, V, Types...>();
}

template <typename System>
std::shared_ptr<System> ECSEngine::find_system()
{
    for (auto &pair : subscribers)
    {
        auto &systems = pair.second;
        for (auto it = systems.begin(); it != systems.end(); ++it)
        {
            std::shared_ptr<System> sp_system = std::dynamic_pointer_cast<System>(*it);
            if (sp_system)
                return sp_system;
        }
    }
    // System not found, return empty shared pointer
    return std::shared_ptr<System>();
}

template <typename System, typename EventType>
bool ECSEngine::is_listener() const
{
    std::type_index type_id = __lz::get_type_index<EventType>();
    return is_listener<System>(type_id);
}

template <typename System>
bool ECSEngine::is_listener(std::type_index type_id) const
{
    auto found = subscribers.find(type_id);
    if (found != subscribers.end())
    {
        // Search for this system in the listeners list 
        auto listeners = found->second;
        for (auto it = listeners.begin(); it != listeners.end(); ++it)
        {
            if (std::dynamic_pointer_cast<System>(*it))
                return true;  // System found
        }
    }
    return false;
}
}  // namespace lz
