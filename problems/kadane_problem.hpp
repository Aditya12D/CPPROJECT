#pragma once
#include "../core/problem.hpp"
#include "../utils/rng.hpp"
#include <vector>
using namespace std;

struct KadaneProblem : Problem {
    vector<int> a;

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
            cur = cur + x;
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
};