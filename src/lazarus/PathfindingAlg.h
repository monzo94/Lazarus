#pragma once

#include <vector>

#include <lazarus/common.h>
#include <lazarus/Heuristics.h>

namespace lz
{
enum class SearchState
{
    NOT_INITIALIZED,
    READY,
    SEARCHING,
    SUCCESS,
    FAILED
};

template <typename Position, typename Map>
class PathfindingAlg
{
public:
    virtual void init(const Position& origin, const Position& goal, Heuristic heuristic=manhattanDistance) = 0;

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

    virtual std::vector<Position> getPath() const { return path; }


protected:
    virtual SearchState searchStep() = 0;

protected:
    SearchState state = SearchState::NOT_INITIALIZED;
    Position& origin;
    Position& goal;
    std::vector<Position> path;
    Heuristic heuristic;
};
}