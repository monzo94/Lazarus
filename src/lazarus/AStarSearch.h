#pragma once

#include <lazarus/PathfindingAlg.h>

namespace lz
{
template <typename Position, typename Map>
class AStarSearch : public PathfindingAlg<Position, Map>
{
public:
    AStarSearch(const Map &map,
                const Position &origin,
                const Position &goal,
                Heuristic<Position> heuristic = manhattanDistance)
        : PathfindingAlg<Position, Map>(map, origin, goal, heuristic)
    {
    }

protected:
    virtual SearchState searchStep()
    {
        if (this->openList.empty())
        {
            // No more nodes in the open list, so the algorithm failed to
            // find a path
            this->state = SearchState::FAILED;
            return this->state;
        }

        // Pop next node in the open list
        Position node = this->openList.top().second;

        // If we reached the goal node, we can finish
        // TODO: Document: Position needs operator== and operator<
        if (node == this->_goal)
        {
            this->state = SearchState::SUCCESS;
            return this->state;
        }

        // Expand neighbours
        // TODO: Document: Map needs neighbours and getCost
        for (auto neighbour : this->map.neighbours(node))
        {
            float cost = this->costToNode[node] + this->map.getCost(neighbour);
            // Also consider visited nodes which would have a
            // smaller cost from this new path
            if (this->previous.find(neighbour) == this->previous.end()
                || cost < this->costToNode[neighbour])
            {
                this->costToNode[neighbour] = cost;
                // Compute score as f = g + h
                float f = cost + this->_heuristic(node, neighbour);
                this->openList.emplace(f, neighbour);
            }
        }

        return SearchState::SEARCHING;
    }
};
}  // namespace lz
