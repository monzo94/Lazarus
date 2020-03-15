#pragma once

#include <lazarus/Heuristics.h>
#include <lazarus/common.h>

#include <algorithm>
#include <map>
#include <queue>
#include <set>
#include <vector>

namespace __lz  // Meant for internal use only
{
template <typename Position>
using QueuePair = std::pair<float, Position>;
}  // namespace __lz

namespace lz
{
/**
 * Defines the state of a pathfinding algorithm.
 */
enum class SearchState
{
    READY,
    SEARCHING,
    SUCCESS,
    FAILED
};

/**
 * Abstract class for heuristic search pathfinding algorithms.
 *
 * @tparam Position The type of position. Must implement the operators `==`, `!=` and `<`.
 * @tparam Map The type of map that the algorithm will use. The requirements
 * depend on the implementation.
 */
template <typename Position, typename Map>
class PathfindingAlg
{
public:
    /**
     * Constructs a new search algorithm, initializing it with the given data.
     *
     * @param map Reference to the map with which the algorithm will work.
     * @param origin Reference to the origin node.
     * @param goal Reference to the goal node.
     * @param heuristic Heuristic for the algorithm to use. By default, it
     * uses the Manhattan distance.
     */
    PathfindingAlg(const Map &map,
                   const Position &origin,
                   const Position &goal,
                   Heuristic<Position> heuristic = manhattan_distance)
        : map(map)
        , origin(origin)
        , goal(goal)
        , heuristic(heuristic)
        , state(SearchState::READY)
    {
    }

    /**
     * Initializes the algorithm with the given data.
     *
     * Sets the origin and goal nodes, and optionally, the heuristic to use.
     */
    virtual void init(const Position &_origin,
                      const Position &_goal,
                      Heuristic<Position> _heuristic = manhattan_distance)
    {
        origin = _origin;
        goal = _goal;
        heuristic = _heuristic;
        state = SearchState::READY;
    }

    SearchState get_state() const
    {
        return state;
    }

    /**
     * Executes a search with previously initialized data.
     *
     * The algorithm must be ready for a search, meaning that it must have been
     * initialized with the @ref init(const Position &origin, const Position &goal,
     * Heuristic<Position> heuristic) method.
     *
     * The algorithm will be executed until completion, that is, until a path is found,
     * or until the algorithm cannot continue because a path does not exist.
     *
     * If the search was successful, the final path will be constructed using the
     * @ref construct_path() method.
     *
     * @return The search state after the execution of the algorithm. It can be either
     * `SearchState::SUCCESS` if a path was found, or `SearchState::FAILED`
     * if a path could not be found.
     */
    virtual SearchState execute()
    {
        if (state != SearchState::READY)
            throw __lz::LazarusException(
                "Tried to execute an uninitialized pathfinding algorithm.");

        // Clear open and closed lists
        closed_list.clear();
        while (!open_list.empty())
            open_list.pop();

        // Clear caches of paths and costs
        previous.clear();
        cost_to_node.clear();

        // Clear old path
        path.clear();

        // Add origin node to the open list, with default values
        open_list.emplace(0.0f, origin);

        // Run search algorithm until we either succeed or fail
        state = SearchState::SEARCHING;
        while (state == SearchState::SEARCHING)
            state = search_step();

        // If search was successful, construct the found path
        if (state == SearchState::SUCCESS)
            construct_path();

        return state;
    }

    /**
     * Initializes the algorithm with the given origin and goal nodes and executes it.
     *
     * The heuristic used will be the one set upon construction or by a previous call to
     * @ref init(const Position &origin, const Position &goal,
     * Heuristic<Position> heuristic).
     *
     * @see execute()
     */
    virtual SearchState execute(const Position &origin, const Position &goal)
    {
        init(origin, goal, heuristic);
        return execute();
    }

    /**
     * Gets the final path from a successful search.
     *
     * The path will start at the next step after the origin, and will end
     * at the goal node.
     *
     * @throws LazarusException If the search has not finished successfully.
     *
     * @return A vector with the ordered final path comnputed by the algorithm.
     */
    virtual std::vector<Position> getPath() const
    {
        if (state != SearchState::SUCCESS)
            throw __lz::LazarusException(
                "Trying to get path from a failed pathfinding search.");
        return path;
    }

protected:
    /**
     * Perform a search step of the pathfinding algorithm.
     *
     * This method is abstract, and is the method that every pathfinding algorithm needs
     * to implement to work correctly.
     *
     * @return The search state after the execution of the search step.
     */
    virtual SearchState search_step() = 0;

private:
    /**
     * Reconstructs the final path from the origin to the goal nodes.
     *
     * The path will start at the step after the origin, and end at the goal node.
     * The constructed path will be stored in the `path` attribute from the class.
     *
     * @throw LazarusException If the search has not finished successfully.
     */
    void construct_path()
    {
        if (state != SearchState::SUCCESS)
            throw __lz::LazarusException(
                "Trying to get path from a failed pathfinding search.");

        // Path starts from the next step after the origin,
        // and finishes at the goal
        Position current = goal;
        while (!(current == origin))
        {
            path.push_back(current);
            current = previous.at(current);
        }
        std::reverse(path.begin(), path.end());
    }

protected:
    const Map &map;
    SearchState state;
    Position origin;
    Position goal;
    std::vector<Position> path;
    Heuristic<Position> heuristic;
    std::map<Position, Position> previous;
    std::map<Position, float> cost_to_node;
    std::set<Position> closed_list;
    std::priority_queue<__lz::QueuePair<Position>,
                        std::vector<__lz::QueuePair<Position>>,
                        std::greater<__lz::QueuePair<Position>>>
        open_list;
};
}  // namespace lz
