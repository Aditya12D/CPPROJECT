#include "core/problem.hpp"
#include "core/runner.cpp"
#include "problems/max_problem.hpp"

int main() {
    MaxProblem p;
    stressTest(p, 1000);
}