#pragma once
#include "../utils/timer.hpp"
#include "problem.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <fstream>

using namespace std;

struct EstimationResult {
    string complexity;
    string confidence;
};

// 🔹 Median calculation for noise reduction
long long getMedian(vector<long long> &v) {
    if (v.empty()) return 0;
    nth_element(v.begin(), v.begin() + v.size() / 2, v.end());
    return v[v.size() / 2];
}

// 🔹 ASCII Chart Helper (Safe for Windows CMD)
void drawChart(const vector<int>& ns, const vector<long long>& times) {
    if (times.empty()) return;
    long long maxTime = *max_element(times.begin(), times.end());
    if (maxTime == 0) maxTime = 1;

    cout << "\n[ Performance Visualization ]\n";
    for (size_t i = 0; i < times.size(); i++) {
        cout << setw(10) << ns[i] << " | ";
        int barWidth = (int)((double)times[i] / maxTime * 40);
        for (int j = 0; j < barWidth; j++) cout << "#";
        cout << " " << times[i] << " ns\n";
    }
    cout << "           " << string(45, '-') << endl;
}

// 🔹 Complexity estimator with Confidence Score
EstimationResult estimateComplexity(const vector<int> &ns, const vector<long long> &times) {
    if (times.size() < 3) return {"Insufficient data", "None"};

    vector<double> ratios;
    for (size_t i = 1; i < times.size(); i++) {
        if (times[i - 1] == 0) continue;
        ratios.push_back((double)times[i] / times[i - 1]);
    }

    if (ratios.empty()) return {"Inconclusive", "Low"};

    double avg = 0;
    for (double r : ratios) avg += r;
    avg /= ratios.size();

    // Variance check for confidence
    double variance = 0;
    for (double r : ratios) variance += (r - avg) * (r - avg);
    variance /= ratios.size();

    string confidence = "High";
    if (variance > 0.5) confidence = "Medium";
    if (variance > 1.5) confidence = "Low";

    if (avg < 1.25) return {"O(1)", confidence};
    if (avg < 2.8) {
        return {(avg > 2.15) ? "O(n log n)" : "O(n)", confidence};
    }
    if (avg < 5.5) return {"O(n^2)", confidence};
    if (avg < 10.0) return {"O(n^3)", confidence};

    return {"O(2^n) or higher", "Medium"};
}

// 🔹 Performance test with CSV Export
void performanceTest(Problem &p) {
    cout << "\n--- Performance Analysis: " << p.getName() << " ---\n";
    cout << "Theoretical Target: " << p.getExpectedComplexity() << "\n";
    cout << "Collecting data points...\n";

    vector<long long> fastTimes;
    vector<int> ns;

    int startN = 100;
    int endN = 102400; 
    
    string name = p.getName();
    if (name.find("Binary") != string::npos) endN = 1048576;
    if (name.find("Dijkstra") != string::npos || name.find("Shortest") != string::npos) endN = 51200;

    for (int n = startN; n <= endN; n *= 2) {
        p.generate_input_with_size(n);

        vector<long long> runs;
        int num_runs = 10; 
        volatile int dummy = 0;

        for (int i = 0; i < num_runs; i++) {
            int batch = (n < 1000 ? 100 : 10);
            long long t = measure([&]() {
                for(int k=0; k<batch; k++) {
                    string res = p.fast();
                    if(res.length() > 1000000) dummy++;
                }
            });
            runs.push_back(t / batch);
        }

        long long med = getMedian(runs);
        fastTimes.push_back(med);
        ns.push_back(n);

        cout << "Testing n = " << setw(10) << n << " ... " << med << " ns\n";
    }

    drawChart(ns, fastTimes);

    string filename = "data/results_" + p.getName() + ".csv";
    replace(filename.begin(), filename.end(), ' ', '_');
    ofstream csv(filename);
    csv << "n,time_ns\n";
    for(size_t i=0; i<ns.size(); i++) {
        csv << ns[i] << "," << fastTimes[i] << "\n";
    }
    csv.close();
    cout << "Results saved to: " << filename << "\n";

    EstimationResult est = estimateComplexity(ns, fastTimes);
    cout << "\n" << string(45, '=') << "\n";
    cout << "          FINAL ANALYSIS REPORT\n";
    cout << string(45, '=') << "\n";
    cout << left << setw(20) << "Problem:" << p.getName() << "\n";
    cout << left << setw(20) << "Theoretical Target:" << p.getExpectedComplexity() << "\n";
    cout << left << setw(20) << "Empirical Observed:" << est.complexity << "\n";
    cout << left << setw(20) << "Confidence Level:" << est.confidence << "\n";
    
    bool match = (est.complexity == p.getExpectedComplexity());
    if (!match) {
        if (est.complexity == "O(n)" && p.getExpectedComplexity() == "O(n log n)") match = true;
        if (est.complexity == "O(n log n)" && p.getExpectedComplexity() == "O(n)") match = true;
        if (est.complexity == "O(n)" && p.getExpectedComplexity() == "O(E log V)") match = true;
        if (est.complexity == "O(n log n)" && p.getExpectedComplexity() == "O(E log V)") match = true;
    }

    cout << left << setw(20) << "Conclusion:";
    if (match) {
        cout << "Matches theoretical complexity ✅\n";
    } else {
        cout << "Deviation from expected behavior ⚠️\n";
    }
    cout << string(45, '=') << "\n";
}