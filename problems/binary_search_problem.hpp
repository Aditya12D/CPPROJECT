#pragma once
#include "../core/problem.hpp"
#include "../utils/rng.hpp"
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;

struct BinarySearchProblem : Problem {
    vector<int> a;
    int target;

    void generate_input() override {
        int n = getInt(1, 20);
        a.resize(n);

        for(int i=0;i<n;i++) a[i] = getInt(1, 50);

        sort(a.begin(), a.end());
        target = getInt(1, 50);
    }

    void generate_input_with_size(int n) override {
        a.resize(n);

        for(int i=0;i<n;i++) a[i] = getInt(1, n*2);

        sort(a.begin(), a.end());
        target = getInt(1, n*2);
    }

    // 🔹 brute: linear search
    string brute() override {
        for(int x : a) {
            if(x == target) return "1";
        }
        return "0";
    }

    // 🔹 fast: binary search
    string fast() override {
        int l = 0, r = a.size()-1;

        while(l <= r) {
            int mid = (l + r) / 2;

            if(a[mid] == target) return to_string(mid);
            else if(a[mid] < target) l = mid + 1;
            else r = mid - 1;
        }

        return "-1";
    }

    string getInput() override {
        string s = to_string(a.size()) + "\n";
        for(int x : a) s += to_string(x) + " ";
        s += "\n" + to_string(target) + "\n";
        return s;
    }

    void loadInput(const string &input) override {
        stringstream ss(input);
        int n;
        ss >> n;

        a.resize(n);
        for(int i=0;i<n;i++) ss >> a[i];

        ss >> target;
    }
};