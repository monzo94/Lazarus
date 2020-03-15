#pragma once

#include <lazarus/ECS/ECSEngine.h>

namespace lz
{
/**
 * Interface for objects that can be updated by the ECS engine when it ticks.
 *
 * Objects which derive from Updateable will need to implement the update
 * method, which will be called by the update method in the ECS engine.
 *
 * @see ECSEngine
 */
class Updateable
{
public:
    virtual ~Updateable() = default;

    /**
     * Virtual update method.
     *
     * Objects implementing this interface can define this method to do
     * something on the entities passed as argument.
     * The ECSEngine will call the update method on all updateables when
     * the update method from the ECSEngine is called.
     */
    virtual void update(ECSEngine &engine) = 0;
};
}  // namespace lz
