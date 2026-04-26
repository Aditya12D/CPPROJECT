#pragma once
#include "../core/problem.hpp"
#include "../generators/array_generator.hpp"

struct MaxProblem : Problem {
    vector<int> a;

    void generate_input() override {
        int n = getInt(1, 20);
        a.resize(n);

        int mode = getInt(0, 4);

        if(mode == 0) {
            for(int i=0;i<n;i++) a[i] = getInt(-100, 100);
        }
        else if(mode == 1) {
            int x = getInt(-10, 10);
            fill(a.begin(), a.end(), x);
        }
        else if(mode == 2) {
            for(int i=0;i<n;i++) a[i] = i;
        }
        else if(mode == 3) {
            for(int i=0;i<n;i++) a[i] = n-i;
        }
        else {
            for(int i=0;i<n;i++) a[i] = (i%2 ? 1000000000 : -1000000000);
        }
    }

    string brute() override {
        int mx = INT_MIN;
        for(int x : a) mx = max(mx, x);
        return to_string(mx);
    }

    string fast() override {
        return to_string(*max_element(a.begin(), a.end()));
    }
};