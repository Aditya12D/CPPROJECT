#pragma once
#include <fstream>
#include "../core/problem.hpp"

void logFailure(const string &input,const string &brute,const string &fast) {
    ofstream fout("data/fail.txt");
    fout<<"input\n";
    fout <<input;
    fout<<"brute output \n";
    fout <<brute;
    fout<<"\nfast output\n";
    fout <<fast;
}