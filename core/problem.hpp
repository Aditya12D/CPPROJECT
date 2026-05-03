#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Problem {
    virtual void generate_input() = 0;
    virtual void generate_input_with_size(int n) = 0;   // 👈 NEW

    virtual string brute() = 0;
    virtual string fast() = 0;

    virtual string getInput() = 0;
    virtual void loadInput(const string &input) = 0;

    virtual ~Problem() {}
};