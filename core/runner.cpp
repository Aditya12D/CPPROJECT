#pragma once
#include "../utils/logger.hpp"
#include "../utils/timer.hpp"

void stressTest(Problem &p, int tests) {
    for(int t = 1; t <= tests; t++) {
        p.generate_input();

        string b = p.brute();
        string f = p.fast();

        if(b != f) {
            cout << "Mismatch found!\n";
            logFailure(p.getInput(),b,f);
            return;
        }
    }
    cout << "All tests passed\n";
}