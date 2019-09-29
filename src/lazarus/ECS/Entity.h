#pragma once

#include <memory>
#include <typeindex>
#include <sstream>
#include <unordered_map>
#include <vector>

#include <lazarus/common.h>

namespace __lz  // Meant for internal use only
{
template <typename T>
std::type_index get_type_index()
{
    return std::type_index(typeid(T));
}

class BaseComponentHandle
{
public:
    virtual ~BaseComponentHandle() = default;
};

template <typename Component>
class ComponentHandle : public BaseComponentHandle
{
public:
    ComponentHandle(std::shared_ptr<Component> comp)
        : component(std::move(comp))
    {
    }

    Component *get() { return component.get(); }

private:
    std::shared_ptr<Component> component;
};
}

namespace lz
{
using Identifier = size_t;

/**
 * An Entity is a collection of components with a unique ID.
 * 
 * An Entity can only have one component of each type at the same time.
 */
class Entity
{
public:
    /**
     * Default constructor.
     * 
     * Sets the ID of the entity to the next available ID.
     */
    Entity();

    /**
     * Returns the ID of the entity.
     */
    Identifier get_id() const { return entity_id; }

    /**
     * Returns whether the entity has a component of type T.
     */
    template <typename Component>
    bool has() const;

    /**
     * Returns whether the entity has all the components of the given types.
     */
    template <typename T, typename V, typename... Types>
    bool has() const;

    /**
     * Attaches a component to the entity.
     * 
     * The component will be constructed with the arguments passed and attached to
     * the entity's pool of components. Therefore, the component must have a
     * constructor that matches the arguments passed.
     * 
     * If the entity already has a component of the specified type, an exception
     * will be thrown.
     */
    template <typename Component, typename... Args>
    void add_component(Args&&... args);

    /**
     * Removes the component of type T from the entity.
     * 
     * If the entity does not have a component of the specified type, an exception
     * is thrown.
     */
    template <typename Component>
    void remove_component();

    /**
     * Returns a pointer to the entity's component of the specified type.
     * 
     * If the entity does not hold a component of that type, a nullptr will be returned.
     */
    template <typename Component>
    Component* get();

    /**
     * Returns whether this entity is marked for deletion upon the next pass of
     * the garbage collector.
     */
    bool is_deleted() const { return deleted; }

    /**
     * Marks the entity for deletion.
     * 
     * An entity marked for deletion will be cleared from memory on the next pass of the
     * ECS engine garbage collector.
     */
    void mark_for_deletion() { deleted = true; }

    /**
     * Returns true if the IDs of the entities are the same.
     */
    bool operator==(const Entity& other);

    /**
     * Returns true if the IDs of the entities are different.
     */
    bool operator!=(const Entity& other);
    
    /**
     * Returns true if the ID of this entity is smaller than the other.
     */
    bool operator<(const Entity& other);

private:
    const Identifier entity_id;
    static Identifier entity_count;  // Keep track of the number of entities to assign new IDs
    std::unordered_map<std::type_index, std::shared_ptr<__lz::BaseComponentHandle>> components;
    bool deleted = false;
};

template <typename Component>
bool Entity::has() const
{
    return components.find(__lz::get_type_index<Component>()) != components.end();
}

template <typename T, typename V, typename... Types>
bool Entity::has() const
{
    return has<T>() && has<V, Types...>();
}

template <typename Component, typename... Args>
void Entity::add_component(Args&&... args)
{
    // Check if the entity already holds a component T
    if (has<Component>())
    {
        std::stringstream msg;
        msg << "Entity " << get_id() << " already holds a component of type "
            << typeid(Component).name();
        throw __lz::LazarusException(msg.str());
    }

    // Construct component and add it to the map
    std::shared_ptr<__lz::BaseComponentHandle> handle(
        new __lz::ComponentHandle<Component>(std::make_shared<Component>(args...))
    );
    components[__lz::get_type_index<Component>()] = std::move(handle);
}

template <typename Component>
void Entity::remove_component()
{
    if (!has<Component>())
    {
        std::stringstream msg;
        msg << "Entity " << get_id() << " does not have a component of type "
            << typeid(Component).name();
        throw __lz::LazarusException(msg.str());
    }

    components.erase(__lz::get_type_index<Component>());
}

template <typename Component>
Component* Entity::get()
{
    auto found = components.find(__lz::get_type_index<Component>());
    if (found == components.end())
        return nullptr;  // TODO: Log this case
    auto comp_handle = dynamic_cast<__lz::ComponentHandle<Component>*>(found->second.get());
    return comp_handle->get();
}
}  // namespace lz
