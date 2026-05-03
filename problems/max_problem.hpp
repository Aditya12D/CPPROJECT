#pragma once
#include "../core/problem.hpp"
#include "../utils/rng.hpp"
#include <vector>
#include <sstream>
#include <algorithm>
#include <climits>
using namespace std;

struct MaxProblem : Problem {
    string getName() const override { return "Find Maximum Element"; }
    string getDescription() const override { return "Find the largest element in an array."; }
    string getDifficulty() const override { return "Easy"; }
    string getExpectedComplexity() const override { return "O(n)"; }

    vector<int> a;

    void generate_input() override {
        int n = getInt(1, 10);
        a.resize(n);
        for(int i=0;i<n;i++) a[i] = getInt(0, 100);
    }

    // 👇 NEW (required)
    void generate_input_with_size(int n) override {
        a.resize(n);
        for(int i=0;i<n;i++) a[i] = getInt(0, 100);
    }

    string brute() override {
        int mx = INT_MIN;
        for(int x : a) mx = max(mx, x);
        return to_string(mx);
    }

    string fast() override {
        return to_string(*max_element(a.begin(), a.end()));
    }

    // 👇 NEW
    string getInput() override {
        string s = to_string(a.size()) + "\n";
        for(int x : a) s += to_string(x) + " ";
        s += "\n";
        return s;
    }

    // 👇 NEW
    void loadInput(const string &input) override {
        stringstream ss(input);
        int n;
        ss >> n;

        a.resize(n);
        for(int i=0;i<n;i++) ss >> a[i];
    }
};