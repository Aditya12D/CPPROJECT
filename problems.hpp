#pragma once
#include "core/problem_registry.hpp"
#include "problems/max_problem.hpp"
#include "problems/kadane_problem.hpp"
#include "problems/graph_problem.hpp"
#include "problems/binary_search_problem.hpp"
#include "problems/circular_subarray_problem.hpp"
#include "problems/dijkstra_problem.hpp"

void registerAllProblems() {
    REGISTER_PROBLEM(MaxProblem);
    REGISTER_PROBLEM(KadaneProblem);
    REGISTER_PROBLEM(GraphProblem);
    REGISTER_PROBLEM(BinarySearchProblem);
    REGISTER_PROBLEM(CircularSubarrayProblem);
    REGISTER_PROBLEM(DijkstraProblem);
}
