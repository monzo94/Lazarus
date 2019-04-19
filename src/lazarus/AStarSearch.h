#pragma once

#include <lazarus/PathfindingAlg.h>

namespace lz
{
template <typename Position, typename Map>
class AStarSearch : public PathfindingAlg<Position, Map>
{
protected:
    virtual SearchState searchStep()
    {
        if (openList.empty())
        {
            // No more nodes in the open list, so the algorithm failed to
            // find a path
            state = SearchState::FAILED;
            return state;
        }

        // Pop next node in the open list
        Position node = openList.top().second;

        // If we reached the goal node, we can finish
        // TODO: Document: Position needs operator== and operator<
        if (node == _goal)
        {
            state = SearchState::SUCCESS;
            return state;
        }

        // Expand neighbours
        // TODO: Document: Map needs neighbours and getCost
        for (auto neighbour : map.neighbours(node))
        {
            float cost = costToNode[node] + map.getCost(neighbour);
            // Also consider visited nodes which would have a
            // smaller cost from this new path
            if (previous.find(neighbour) == previous.end() || cost < costToNode[neighbour])
            {
                costToNode[neighbour] = cost;
                // Compute score as f = g + h
                float f = cost + _heuristic(neighbour);
                openList.emplace(f, neighbour);
            }
        }

        return SearchState::SEARCHING;
    }
};
}  // namespace lz