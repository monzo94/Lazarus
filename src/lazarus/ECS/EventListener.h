#pragma once

namespace __lz  // Meant for internal use only
{
class BaseEventListener
{
public:
    // Add virtual destructor to make class polymorphic
    virtual ~BaseEventListener() = default;
};
}

namespace lz
{
class ECSEngine;

/**
 * Interface for objects that react to events of a certain type.
 * 
 * Objects which derive from EventListener will need to implement the receive
 * method, which will be called when an event of the same type is emitted by the
 * ECS engine, as long as the listener is subscribed to the list of listeners
 * of that event type in the ECS engine.
 * 
 * Usually, systems will also be event listeners, so they can implement this
 * interface.
 * 
 * @see ECSEngine
 * @see BaseSystem
 */
template <typename EventType>
class EventListener : public virtual __lz::BaseEventListener
{
public:
    // Add virtual destructor to make class polymorphic
    virtual ~EventListener() = default;

    /**
     * Called when the EventListener receives an event from the ECS engine.
     * 
     * @param engine Reference to the ECS engine the listener lives in.
     * @param event The event that is emitted by the ECS engine and received by this.
     */
    virtual void receive(ECSEngine& engine, const EventType& event) = 0;
};
}  // namespace lz
