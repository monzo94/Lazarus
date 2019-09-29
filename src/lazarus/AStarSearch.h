#pragma once

#include <lazarus/PathfindingAlg.h>

namespace lz
{
/**
 * Implementation of the A* pathfinding algorithm.
 * 
 * The A* algorithm uses a heuristic to speed up the search of an optimal path between
 * any two given nodes.
 * 
 * The algorithm will only produce optimal paths (that is, minimal distance paths)
 * reliably if the heuristic never overestimates the actual distance of an optimal path
 * from any node to the goal node.
 * 
 * @tparam Position The type of position. Must implement the operators `==`, `!=` and `<`.
 * @tparam Map THe type of map that the algorithm will use. Must implement the methods
 * `get_cost(const Position&)` and `neighbours(const Position&)`.
 */
template <typename Position, typename Map>
class AStarSearch : public PathfindingAlg<Position, Map>
{
public:
    /**
     * Initializes a new A* search algorithm with the given data.
     * 
     * @param map Reference to the map with which the algorithm will work.
     * @param origin Reference to the origin node.
     * @param goal Reference to the goal node.
     * @param heuristic Heuristic for the algorithm to use. By default, it
     * uses the Manhattan distance.
     */
    AStarSearch(const Map &map,
                const Position &origin,
                const Position &goal,
                Heuristic<Position> heuristic = manhattan_distance)
        : PathfindingAlg<Position, Map>(map, origin, goal, heuristic)
    {
    }

protected:
    /**
     * Perform a search step of the A* algorithm.
     * 
     * @return The search state after the execution of the search step.
     */
    virtual SearchState search_step()
    {
        if (this->open_list.empty())
        {
            // No more nodes in the open list, so the algorithm failed to
            // find a path
            this->state = SearchState::FAILED;
            return this->state;
        }

        // Pop next node in the open list
        Position node = this->open_list.top().second;
        this->open_list.pop();

        // If we reached the goal node, we can finish
        // TODO: Document: Position needs operator== and operator<
        if (node == this->goal)
        {
            this->state = SearchState::SUCCESS;
            return this->state;
        }

        // Expand neighbours
        // TODO: Document: Map needs neighbours and get_cost
        for (Position neighbour : this->map.neighbours(node))
        {
            float cost = this->cost_to_node[node] + this->map.get_cost(neighbour);
            // Also consider visited nodes which would have a
            // smaller cost from this new path
            if (this->previous.find(neighbour) == this->previous.end()
                || cost < this->cost_to_node[neighbour])
            {
                this->cost_to_node[neighbour] = cost;
                this->previous.insert(std::pair<Position, Position>(neighbour, node));
                // Compute score as f = g + h
                float f = cost + this->heuristic(node, neighbour);
                this->open_list.emplace(f, neighbour);
            }
        }

        return SearchState::SEARCHING;
    }
};
}  // namespace lz
