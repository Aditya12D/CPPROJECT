#include "core/problem.hpp"
#include "core/runner.cpp"
#include "problems/max_problem.hpp"
#include "problems/kadane_problem.hpp"

int main() {
    KadaneProblem p;
    stressTest(p, 1000);
}