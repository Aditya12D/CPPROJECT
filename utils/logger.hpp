#pragma once
#include <fstream>
#include "../core/problem.hpp"

void logFailure(Problem &p) {
    ofstream fout("data/fail.txt");
    fout << "Fail case captured\n";
}