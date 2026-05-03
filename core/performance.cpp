#pragma once
#include "../utils/timer.hpp"
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

// 🔹 Complexity estimator
string estimateComplexity(const vector<long long> &times) {
    vector<double> ratios;

    for(int i = 1; i < times.size(); i++) {
        if(times[i-1] == 0) continue;
        ratios.push_back((double)times[i] / times[i-1]);
    }

    if(ratios.empty()) return "Insufficient data";

    double avg = 0;
    for(double r : ratios) avg += r;
    avg /= ratios.size();

    if(abs(avg - 1) < 0.5) return "O(1)";
    if(abs(avg - 2) < 0.7) return "O(n)";
    if(abs(avg - 2.2) < 0.7) return "O(n log n)";
    if(abs(avg - 4) < 1.5) return "O(n^2)";
    if(abs(avg - 8) < 3) return "O(n^3)";

    return "Unknown";
}

// 🔹 Performance test
template<typename P>
void performanceTest(P &p) {
    cout << "n\tfast(us)\n";

    vector<long long> fastTimes;

    for(int n = 100; n <= 100000; n *= 2) {
        p.generate_input_with_size(n);

        long long total = 0;
        int runs = 500;

        string result;

        for(int i = 0; i < runs; i++) {
            total += measure([&]() {
                result = p.fast();
            });
        }
        if(result == "__dummy__") cout << "";
        long long t = total / runs;

        fastTimes.push_back(t);

        cout << n << "\t" << t << "\n";
    }

    // 🔹 Estimate complexity
    string complexity = estimateComplexity(fastTimes);

    cout << "\nEstimated Complexity (fast): " << complexity << "\n";
}