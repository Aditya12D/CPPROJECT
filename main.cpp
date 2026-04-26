#include "core/problem.hpp"
#include "core/runner.cpp"
#include "problems/kadane_problem.hpp"
#include <fstream>

int main() {
    KadaneProblem p;

    int mode;
    cout << "Enter mode (1 = stress, 2 = replay): ";
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

        cout << "Replaying test:\n";
        cout << input << "\n";

        cout << "Brute: " << p.brute() << "\n";
        cout << "Fast: " << p.fast() << "\n";
    }
}