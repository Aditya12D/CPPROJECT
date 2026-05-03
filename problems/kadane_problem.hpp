#pragma once
#include "../core/problem.hpp"
#include "../utils/rng.hpp"
#include <vector>
using namespace std;

struct KadaneProblem : Problem {
    string getName() const override { return "Maximum Subarray Sum (Kadane's)"; }
    string getDescription() const override { return "Find the contiguous subarray with the largest sum."; }
    string getDifficulty() const override { return "Medium"; }
    string getExpectedComplexity() const override { return "O(n)"; }

    vector<int> a;
    void generate_input_with_size(int n) override {
        a.resize(n);

        int mode = getInt(0, 3);

        if(mode == 0) {
            for(int i=0;i<n;i++) a[i] = getInt(-10, 10);
        }
        else if(mode == 1) {
            int x = getInt(-5, 5);
            fill(a.begin(), a.end(), x);
        }
        else if(mode == 2) {
            for(int i=0;i<n;i++) a[i] = (i%2 ? 10 : -10);
        }
        else {
            for(int i=0;i<n;i++) a[i] = i - n/2;
        }
    }
    void generate_input() override {
        int n = getInt(1, 15);
        a.resize(n);

        for(int i=0;i<n;i++) {
            a[i] = getInt(-10, 10);
        }
    }

    string brute() override {
        int best = INT_MIN;

        for(int i=0;i<a.size();i++) {
            int sum = 0;
            for(int j=i;j<a.size();j++) {
                sum += a[j];
                best = max(best, sum);
            }
        }

        return to_string(best);
    }

    string fast() override {
        int cur = 0, best = INT_MIN;

        for(int x : a) {
            cur = max(x,cur + x);
            best = max(best, cur);
        }

        return to_string(best);
    }
    string getInput() override {
        string s = to_string(a.size()) + "\n";

        for(int x : a) {
            s += to_string(x) + " ";
        }

        s += "\n";
        return s;
    }
    void loadInput(const string &input) {
        stringstream ss(input);
        int n;
        ss >> n;

        a.resize(n);
        for(int i=0;i<n;i++) ss >> a[i];
    }
};