#pragma once
#include <fstream>
#include "../core/problem.hpp"

void logFailure(const string &input, const string &brute, const string &fast) {
    ofstream fin("data/fail_input.txt");
    fin << input;   // ONLY input

    ofstream flog("data/fail_log.txt");
    flog << "=== INPUT ===\n" << input << "\n";
    flog << "=== BRUTE ===\n" << brute << "\n";
    flog << "=== FAST ===\n" << fast << "\n";
}