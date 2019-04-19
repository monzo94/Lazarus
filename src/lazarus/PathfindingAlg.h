#pragma once

#include <vector>

#include <lazarus/common.h>
#include <lazarus/Heuristics.h>

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
    PathfindingAlg(const Position &origin,
                   const Position &goal,
                   Heuristic<Position> heuristic = manhattanDistance)
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
        if (state == SearchState::NOT_INITIALIZED)
            throw __lz::LazarusException("Pathfinding algorithm not initialized.");

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
    SearchState state = SearchState::NOT_INITIALIZED;
    Position& _origin;
    Position& _goal;
    std::vector<Position> path;
    Heuristic<Position> _heuristic;
};
}  // namespace lz