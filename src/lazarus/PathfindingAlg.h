#pragma once

#include <map>
#include <queue>
#include <set>
#include <vector>

#include <lazarus/common.h>
#include <lazarus/Heuristics.h>

namespace __lz  // Meant for internal use only
{
template <typename Position>
using QueuePair = std::pair<float, Position>;
}  // namespace __lz

namespace lz
{
enum class SearchState
{
    READY,
    SEARCHING,
    SUCCESS,
    FAILED
};

template <typename Position, typename Map>
class PathfindingAlg
{
public:
    PathfindingAlg(const Map &map,
                   const Position &origin,
                   const Position &goal,
                   Heuristic<Position> heuristic = manhattanDistance)
        : map(map)
    {
        init(origin, goal, heuristic);
    }

    virtual void init(const Position &origin,
                      const Position &goal,
                      Heuristic<Position> heuristic = manhattanDistance)
    {
        _origin = origin;
        _goal = goal;
        _heuristic = heuristic;
        state = SearchState::READY;
    }

    SearchState getState() const { return state; }

    virtual SearchState execute()
    {
        // Clear open and closed lists
        closedList.clear();
        while (!openList.empty())
            openList.pop();

        // Clear caches of paths and costs
        previous.clear();
        costToNode.clear();
        
        // Add origin node to the open list, with default values
        openList.emplace(0.0f, _origin);
        
        // Run search algorithm until we either succeed or fail
        state = SearchState::SEARCHING;
        while (state == SearchState::SEARCHING)
            state = searchStep();
        return state;
    }

    virtual std::vector<Position> getPath() const
    {
        if (state != SearchState::SUCCESS)
            throw __lz::LazarusException("Trying to get path from failed pathfinding.");
        return path;
    }


protected:
    virtual SearchState searchStep() = 0;

protected:
    Map& map;
    SearchState state = SearchState::NOT_INITIALIZED;
    Position& _origin;
    Position& _goal;
    std::vector<Position> path;
    Heuristic<Position> _heuristic;
    std::map<Position, Position> previous;
    std::map<Position, float> costToNode;
    std::set<Position> closedList;
    std::priority_queue<__lz::QueuePair<Position>,
                        std::vector<__lz::QueuePair<Position>>,
                        std::greater<__lz::QueuePair<Position>>> openList;
};
}  // namespace lz