#pragma once
#include "../utils/timer.hpp"
#include "../utils/colors.hpp"
#include "problem.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <fstream>

using namespace std;

struct EstimationResult { string complexity, confidence; };

long long getMedian(vector<long long>& v) {
    if (v.empty()) return 0;
    nth_element(v.begin(), v.begin() + v.size() / 2, v.end());
    return v[v.size() / 2];
}

// ─── Colored ASCII Bar Chart ──────────────────────────────────────────────
void drawChart(const vector<int>& ns, const vector<long long>& times) {
    if (times.empty()) return;
    long long maxTime = *max_element(times.begin(), times.end());
    if (maxTime == 0) maxTime = 1;

    cout << "\n  " << Color::BOLD << Color::BLUE << "Performance Chart\n"
         << Color::RESET << Color::GRAY << "  " << string(55, '-') << Color::RESET << "\n";

    // Color gradient based on relative time
    const char* barColors[] = { Color::GREEN, Color::GREEN, Color::YELLOW, Color::YELLOW, Color::RED };

    for (size_t i = 0; i < times.size(); i++) {
        int barW = (int)((double)times[i] / maxTime * 35);
        float rel = (float)i / max(1, (int)times.size() - 1);
        const char* col = barColors[(int)(rel * 4)];

        cout << "  " << Color::GRAY << setw(8) << ns[i] << Color::RESET << " | ";
        cout << col;
        for (int j = 0; j < barW; j++) cout << "█";
        cout << Color::RESET;
        cout << Color::GRAY << " " << times[i] << " ns\n" << Color::RESET;
    }
    cout << "  " << Color::GRAY << string(55, '-') << Color::RESET << "\n";
}

// ─── Complexity Estimator ─────────────────────────────────────────────────
EstimationResult estimateComplexity(const vector<int>& ns, const vector<long long>& times) {
    if (times.size() < 3) return { "Insufficient data", "None" };

    vector<double> ratios;
    for (size_t i = 1; i < times.size(); i++) {
        if (times[i - 1] == 0) continue;
        ratios.push_back((double)times[i] / times[i - 1]);
    }
    if (ratios.empty()) return { "Inconclusive", "Low" };

    double avg = 0;
    for (double r : ratios) avg += r;
    avg /= ratios.size();

    double variance = 0;
    for (double r : ratios) variance += (r - avg) * (r - avg);
    variance /= ratios.size();

    string confidence = variance > 1.5 ? "Low" : variance > 0.5 ? "Medium" : "High";

    if (avg < 1.25) return { "O(1)",          confidence };
    if (avg < 2.15) return { "O(n)",          confidence };
    if (avg < 2.80) return { "O(n log n)",    confidence };
    if (avg < 5.50) return { "O(n^2)",        confidence };
    if (avg < 10.0) return { "O(n^3)",        confidence };
    return { "O(2^n) or higher", "Medium" };
}

// ─── Performance Test ─────────────────────────────────────────────────────
void performanceTest(Problem& p) {
    string title = "  PERFORMANCE ANALYSIS: " + p.getName() + "  ";
    int w = max(52, (int)title.size() + 2);
    cout << "\n" << Color::BOLD << Color::MAGENTA
         << "  " << string(w, '=') << "\n"
         << "  " << title << string(w - (int)title.size(), ' ') << "\n"
         << "  " << string(w, '=') << "\n"
         << Color::RESET;

    cout << Color::GRAY
         << "  Target: " << Color::RESET << Color::BOLD << p.getExpectedComplexity()
         << Color::RESET << "\n\n";

    vector<long long> fastTimes;
    vector<int>       ns;

    int endN = 102400;
    string name = p.getName();
    if (name.find("Binary")   != string::npos) endN = 1048576;
    if (name.find("Dijkstra") != string::npos || name.find("Shortest") != string::npos) endN = 51200;

    cout << Color::GRAY << "  " << setw(10) << "n"
         << "  " << setw(12) << "median (ns)"
         << "  " << "runs\n"
         << "  " << string(38, '-') << Color::RESET << "\n";

    for (int n = 100; n <= endN; n *= 2) {
        p.generate_input_with_size(n);

        vector<long long> runs;
        int batch = (n < 1000 ? 100 : 10);
        volatile int dummy = 0;

        for (int i = 0; i < 10; i++) {
            long long t = measure([&]() {
                for (int k = 0; k < batch; k++) {
                    string res = p.fast();
                    if (res.length() > 1000000) dummy++;
                }
            });
            runs.push_back(t / batch);
        }

        long long med = getMedian(runs);
        fastTimes.push_back(med);
        ns.push_back(n);

        // Color rows: green → yellow → red as n grows
        float rel = (float)ns.size() / 10.0f;
        const char* col = rel < 0.4 ? Color::GREEN : rel < 0.7 ? Color::YELLOW : Color::RED;

        cout << "  " << col << setw(10) << n << Color::RESET
             << "  " << Color::BOLD << setw(12) << med << Color::RESET
             << " ns\n";
    }

    drawChart(ns, fastTimes);

    // Save CSV
    string filename = "data/results_" + p.getName() + ".csv";
    replace(filename.begin(), filename.end(), ' ', '_');
    ofstream csv(filename);
    csv << "n,time_ns\n";
    for (size_t i = 0; i < ns.size(); i++) csv << ns[i] << "," << fastTimes[i] << "\n";
    csv.close();

    EstimationResult est = estimateComplexity(ns, fastTimes);
    bool match = (est.complexity == p.getExpectedComplexity());
    if (!match) {
        if ((est.complexity == "O(n)"       && p.getExpectedComplexity() == "O(n log n)") ||
            (est.complexity == "O(n log n)" && p.getExpectedComplexity() == "O(n)")       ||
            (est.complexity == "O(n)"       && p.getExpectedComplexity() == "O(E log V)") ||
            (est.complexity == "O(n log n)" && p.getExpectedComplexity() == "O(E log V)"))
            match = true;
    }

    // ── Final Report ──
    cout << "\n  " << Color::BOLD << Color::BLUE << "FINAL ANALYSIS REPORT\n" << Color::RESET
         << Color::GRAY << "  " << string(46, '=') << Color::RESET << "\n";

    cout << "  " << Color::GRAY << setw(22) << left << "Problem:"    << Color::RESET << p.getName()                  << "\n";
    cout << "  " << Color::GRAY << setw(22) << left << "Target:"     << Color::RESET << p.getExpectedComplexity()     << "\n";
    cout << "  " << Color::GRAY << setw(22) << left << "Observed:"   << Color::RESET << Color::BOLD << est.complexity << Color::RESET << "\n";
    cout << "  " << Color::GRAY << setw(22) << left << "Confidence:" << Color::RESET << est.confidence                << "\n";
    cout << "  " << Color::GRAY << setw(22) << left << "CSV saved:"  << Color::RESET << filename                      << "\n";
    cout << "  " << Color::GRAY << setw(22) << left << "Conclusion:" << Color::RESET;

    if (match) cout << Color::GREEN << Color::BOLD << "  Matches expected complexity\n" << Color::RESET;
    else        cout << Color::RED   << Color::BOLD << "  Deviates from expected\n"      << Color::RESET;

    cout << Color::GRAY << "  " << string(46, '=') << Color::RESET << "\n";
}