#pragma once
#include <fstream>
#include "../core/problem.hpp"

void logFailure(const string &input) {
    ofstream fout("data/fail.txt");
    fout <<input;
}