#include "core/runner.cpp"
#include "core/performance.cpp"
#include "problems/max_problem.hpp"
#include "problems/kadane_problem.hpp"
#include "problems/graph_problem.hpp"
#include "problems/binary_search_problem.hpp"
#include <fstream>

int main() {
    BinarySearchProblem p;

    int mode;
    cout << "1 = stress, 2 = replay, 3 = performance\n";
    cin >> mode;

    if(mode == 1) {
        stressTest(p, 1000);
    }
    else if(mode == 2) {
        ifstream fin("data/fail_input.txt");

        if(!fin) {
            cout << "No fail file found\n";
            return 0;
        }

        string input((istreambuf_iterator<char>(fin)),
                      istreambuf_iterator<char>());

        p.loadInput(input);

        cout << "Brute: " << p.brute() << "\n";
        cout << "Fast: " << p.fast() << "\n";
    }
    else if(mode == 3) {
        performanceTest(p);
    }
}