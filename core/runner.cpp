#pragma once
#include "../utils/logger.hpp"
#include "../utils/timer.hpp"
#include "problem.hpp"
#include <iostream>
#include <chrono>

void stressTest(Problem &p, int tests) {
    cout << "\n--- Stress Testing: " << p.getName() << " ---\n";
    cout << "Running " << tests << " test cases...\n";
    
    auto start = chrono::high_resolution_clock::now();

    for(int t = 1; t <= tests; t++) {
        p.generate_input();

        string b = p.brute();
        string f = p.fast();

        if(b != f) {
            cout << "\n[!] MISMATCH DETECTED at test #" << t << "\n";
            cout << "----------------------------------------\n";
            cout << "Input Snapshot (first 100 chars):\n" << p.getInput().substr(0, 100) << (p.getInput().length() > 100 ? "..." : "") << "\n";
            cout << "Brute Output: " << b << "\n";
            cout << "Fast Output:  " << f << "\n";
            cout << "----------------------------------------\n";
            cout << "Saving full input to data/fail_input.txt for replay.\n";
            
            logFailure(p.getInput(), b, f);
            return;
        }

        if(tests >= 10 && (t % (tests / 10) == 0 || t == tests)) {
            float progress = (float)t / tests * 100;
            cout << "\rProgress: [" << string(t * 20 / tests, '=') << string(20 - t * 20 / tests, ' ') << "] " << (int)progress << "%" << flush;
        }
    }

    auto end = chrono::high_resolution_clock::now();
    double elapsed = chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000.0;

    cout << "\n\n✅ ALL TESTS PASSED SUCCESSFULLY!\n";
    cout << "Performance: " << (int)(tests / (elapsed + 0.001)) << " tests/sec\n";
    cout << "Total Time:  " << elapsed << "s\n";
}