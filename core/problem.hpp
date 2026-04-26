#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Problem {
    virtual void generate_input() = 0;
    virtual string brute() = 0;
    virtual string fast() = 0;
    virtual string getInput()=0;
};