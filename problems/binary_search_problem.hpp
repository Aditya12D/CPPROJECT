#pragma once
#include "../core/problem.hpp"
#include "../utils/rng.hpp"
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;

struct BinarySearchProblem : Problem {
    string getName() const override { return "Binary Search"; }
    string getDescription() const override { return "Find the index of a target element in a sorted array."; }
    string getDifficulty() const override { return "Easy"; }
    string getExpectedComplexity() const override { return "O(log n)"; }

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
        for(int i=0; i<a.size(); i++) {
            if(a[i] == target) return to_string(i);
        }
        return "-1";
    }

    // 🔹 fast: binary search (find first occurrence)
    string fast() override {
        int l = 0, r = a.size()-1;
        int ans = -1;

        while(l <= r) {
            int mid = l + (r - l) / 2;

            if(a[mid] == target) {
                ans = mid;
                r = mid - 1; // Keep looking left
            }
            else if(a[mid] < target) l = mid + 1;
            else r = mid - 1;
        }

        return to_string(ans);
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