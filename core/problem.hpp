#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <climits>
#include <queue>
#include <set>
#include <map>
#include <cmath>
#include <numeric>
using namespace std;

struct Problem {
    // 🔹 Metadata
    virtual string getName() const = 0;
    virtual string getDescription() const = 0;
    virtual string getDifficulty() const = 0; // "Easy", "Medium", "Hard"
    virtual string getExpectedComplexity() const = 0; // "O(n)", "O(log n)", etc.

    // 🔹 Input Generation
    virtual void generate_input() = 0;
    virtual void generate_input_with_size(int n) = 0;

    // 🔹 Core Algorithms
    virtual string brute() = 0;
    virtual string fast() = 0;

    // 🔹 Data Management
    virtual string getInput() = 0;
    virtual void loadInput(const string &input) = 0;

    virtual ~Problem() {}
};