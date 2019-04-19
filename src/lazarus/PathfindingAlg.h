#pragma once

#include <queue>
#include <set>
#include <vector>

#include <lazarus/common.h>
#include <lazarus/Heuristics.h>

namespace __lz  // Meant for internal use only
{
template <typename Position>
struct Node
{
    Node(const Position& pos)
        : position(pos)
        , g(0.0f)
        , h(0.0f)
        , f(0.0f)
    {
    }

    Position& position;
    float g;  // g-score
    float h;  // h-score or heuristic
    float f;  // f-score
};

template <typename Position>
using QueuePair = std::pair<float, Node<Position>>;
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
        // Clear open and closed lists
        closedList.clear();
        while (!openList.empty())
            openList.pop();
        
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
    SearchState state = SearchState::NOT_INITIALIZED;
    Position& _origin;
    Position& _goal;
    std::vector<Position> path;
    Heuristic<Position> _heuristic;
    std::set<Position> closedList;
    std::priority_queue<__lz::QueuePair, std::vector<__lz::QueuePair>,
                        std::greater<__lz::QueuePair>> openList;
};
}  // namespace lz