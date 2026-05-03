#pragma once
#include "../core/problem.hpp"
#include "../generators/array_generator.hpp"
#include "../generators/edge_cases.hpp"
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

struct CircularSubarrayProblem : Problem {
    string getName() const override { return "Max Circular Subarray Sum"; }
    string getDescription() const override { return "Find the maximum possible sum of a non-empty subarray of a circular array."; }
    string getDifficulty() const override { return "Hard"; }
    string getExpectedComplexity() const override { return "O(n)"; }

    vector<int> a;

    void generate_input() override {
        int n = Generator::randomInt(1, 20);
        a = Generator::genArray(n, -20, 20);
    }

    void generate_input_with_size(int n) override {
        a = Generator::genArray(n, -100, 100);
    }

    // 🔹 brute: O(n^2)
    string brute() override {
        int n = a.size();
        int max_sum = -1e9;
        for(int i=0; i<n; i++) {
            int current_sum = 0;
            for(int j=0; j<n; j++) {
                current_sum += a[(i + j) % n];
                max_sum = max(max_sum, current_sum);
            }
        }
        return to_string(max_sum);
    }

    // 🔹 fast: O(n) using Kadane's variation
    string fast() override {
        int n = a.size();
        auto kadane = [](const vector<int>& v) {
            int max_so_far = -1e9, max_ending_here = 0;
            for(int x : v) {
                max_ending_here += x;
                max_so_far = max(max_so_far, max_ending_here);
                max_ending_here = max(max_ending_here, 0);
            }
            return max_so_far;
        };

        int max_kadane = kadane(a);
        
        int total_sum = 0;
        vector<int> inverted_a(n);
        for(int i=0; i<n; i++) {
            total_sum += a[i];
            inverted_a[i] = -a[i];
        }

        int max_wrap = total_sum + kadane(inverted_a);

        // If all elements are negative, max_wrap will be 0 (total_sum - total_sum)
        // in that case we must take the maximum single element (max_kadane)
        if(max_wrap == 0 && max_kadane < 0) return to_string(max_kadane);

        return to_string(max(max_kadane, max_wrap));
    }

    string getInput() override {
        string s = to_string(a.size()) + "\n";
        for(int x : a) s += to_string(x) + " ";
        return s + "\n";
    }

    void loadInput(const string &input) override {
        stringstream ss(input);
        int n; ss >> n;
        a.resize(n);
        for(int i=0; i<n; i++) ss >> a[i];
    }
};
