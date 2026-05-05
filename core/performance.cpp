#pragma once
#include "../utils/timer.hpp"
#include "../utils/colors.hpp"
#include "problem.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <set>

using namespace std;

// ─── Stats helpers ────────────────────────────────────────────────────────────
long long getMedian(vector<long long> v) {
    if (v.empty()) return 0;
    nth_element(v.begin(), v.begin() + v.size() / 2, v.end());
    return v[v.size() / 2];
}

double getMean(const vector<long long>& v) {
    if (v.empty()) return 0;
    double s = 0;
    for (long long x : v) s += x;
    return s / v.size();
}

// ─── Complexity estimator ─────────────────────────────────────────────────────
// Uses log-log OLS regression (slope in log2 T vs log2 n space).
// Also tests an explicit O(log n) linear fit for sub-linear cases.
struct EstimationResult {
    string complexity;   // e.g. "O(n log n)"
    string confidence;   // "High" / "Medium" / "Low"
    double avgRatio;     // mean T(2n)/T(n)
    bool   usedOpCount;  // true if estimated from operation counts
};

EstimationResult estimateComplexity(const vector<int>& ns,
                                    const vector<long long>& vals,
                                    bool isOpCount) {
    int sz = (int)vals.size();
    if (sz < 3) return { "Insufficient data", "None", 0, isOpCount };

    // Build log2-space vectors
    vector<double> logN(sz), logV(sz);
    for (int i = 0; i < sz; i++) {
        logN[i] = log2((double)ns[i]);
        logV[i] = log2((double)max(1LL, vals[i]));
    }

    // OLS slope in log-log space
    double mN = 0, mV = 0;
    for (int i = 0; i < sz; i++) { mN += logN[i]; mV += logV[i]; }
    mN /= sz; mV /= sz;

    double num = 0, den = 0;
    for (int i = 0; i < sz; i++) {
        num += (logN[i] - mN) * (logV[i] - mV);
        den += (logN[i] - mN) * (logN[i] - mN);
    }
    double slope = (den == 0) ? 0 : num / den;

    // R² for confidence
    double ssTot = 0, ssRes = 0;
    for (int i = 0; i < sz; i++) {
        double pred = mV + slope * (logN[i] - mN);
        ssTot += (logV[i] - mV) * (logV[i] - mV);
        ssRes += (logV[i] - pred) * (logV[i] - pred);
    }
    double r2 = (ssTot < 1e-9) ? 1.0 : 1.0 - ssRes / ssTot;
    string confidence = r2 > 0.92 ? "High" : r2 > 0.75 ? "Medium" : "Low";

    // Average doubling ratio
    double avgRatio = 0; int rc = 0;
    for (int i = 1; i < sz; i++) {
        if (vals[i-1] > 0) { avgRatio += (double)vals[i] / vals[i-1]; rc++; }
    }
    if (rc) avgRatio /= rc;

    // ── Special test: does T ~ a * log2(n) fit better? (pure O(log n)) ──
    {
        double mx2 = 0, my2 = 0;
        for (int i = 0; i < sz; i++) { mx2 += logN[i]; my2 += (double)vals[i]; }
        mx2 /= sz; my2 /= sz;
        double n2 = 0, d2 = 0;
        for (int i = 0; i < sz; i++) {
            n2 += (logN[i] - mx2) * ((double)vals[i] - my2);
            d2 += (logN[i] - mx2) * (logN[i] - mx2);
        }
        double a = (d2 == 0) ? 0 : n2 / d2;
        double ssR2 = 0, ssT2 = 0;
        for (int i = 0; i < sz; i++) {
            double pred = my2 + a * (logN[i] - mx2);
            ssR2 += ((double)vals[i] - pred) * ((double)vals[i] - pred);
            ssT2 += ((double)vals[i] - my2)  * ((double)vals[i] - my2);
        }
        double r2log = (ssT2 < 1e-9) ? 1.0 : 1.0 - ssR2 / ssT2;
        // Use log-linear fit if it's clearly better than power-law AND slope < 0.55
        if (r2log > 0.90 && slope < 0.55 && a > 0) {
            string conf2 = r2log > 0.92 ? "High" : r2log > 0.75 ? "Medium" : "Low";
            return { "O(log n)", conf2, avgRatio, isOpCount };
        }
    }

    // ── Map slope to complexity class ──
    // Boundaries tuned for both timing and op-count data:
    //   O(1):          slope < 0.10   ratio ≈ 1.0
    //   O(log n):      slope < 0.55   ratio ≈ 1.0-1.10
    //   O(n):          slope < 1.10   ratio ≈ 2.0         ← true linear
    //   O(n log n):    slope < 1.55   ratio ≈ 2.1-2.3     ← captures E log V too
    //   O(n log²n):    slope < 1.75   ratio ≈ 2.3-2.6
    //   O(n^2):        slope < 2.50   ratio ≈ 4.0
    //   O(n^3):        slope < 3.50   ratio ≈ 8.0
    string cplx;
    if      (slope < 0.10)  cplx = "O(1)";
    else if (slope < 0.55)  cplx = "O(log n)";
    else if (slope < 1.10)  cplx = "O(n)";
    else if (slope < 1.55)  cplx = "O(n log n)";
    else if (slope < 1.80)  cplx = "O(n log\xC2\xB2 n)";
    else if (slope < 2.50)  cplx = "O(n^2)";
    else if (slope < 3.50)  cplx = "O(n^3)";
    else                    cplx = "O(2^n) or higher";

    return { cplx, confidence, avgRatio, isOpCount };
}

// ─── Fuzzy match: are observed and expected "close enough"? ──────────────────
// NOTE: O(1) and O(log n) are intentionally NOT equated — if timing noise
// causes O(log n) to look like O(1), op-count mode should be used instead.
bool fuzzyMatch(const string& observed, const string& expected) {
    if (observed == expected) return true;
    // Adjacent complexity classes that are genuinely hard to distinguish
    static const set<pair<string,string>> ok = {
        {"O(n)",        "O(n log n)"},
        {"O(n log n)",  "O(n)"},
        // O(V+E) with sparse E=O(n) collapses to O(n) — both are correct
        {"O(n)",        "O(V + E)"},
        {"O(V + E)",    "O(n)"},
        {"O(n log n)",  "O(V + E)"},
        // Dijkstra O(E log V): with E=n·logn, weighted ops grow as n·(logn)²
        // which OLS maps to either O(n log n) or O(n log² n) depending on n range
        {"O(n log n)",  "O(E log V)"},
        {"O(n log\xC2\xB2 n)", "O(E log V)"},
        {"O(n log n)",  "O(n log\xC2\xB2 n)"},
        {"O(n log\xC2\xB2 n)", "O(n log n)"},
    };
    return ok.count({observed, expected}) > 0;
}

// ─── Colored ASCII Bar Chart ──────────────────────────────────────────────────
void drawChart(const vector<int>& ns,
               const vector<long long>& medians,
               const vector<double>& avgs,
               const string& unit) {
    if (medians.empty()) return;
    long long maxVal = *max_element(medians.begin(), medians.end());
    if (maxVal == 0) maxVal = 1;

    cout << "\n  " << Color::BOLD << Color::BLUE << "Performance Chart  "
         << Color::DIM << "(Median | Avg  unit=" << unit << ")\n"
         << Color::RESET
         << Color::GRAY << "  " << string(62, '-') << Color::RESET << "\n";

    const char* barColors[] = { Color::GREEN, Color::GREEN, Color::YELLOW, Color::YELLOW, Color::RED };

    for (size_t i = 0; i < medians.size(); i++) {
        int barW = (int)((double)medians[i] / maxVal * 30);
        float rel = (float)i / max(1, (int)medians.size() - 1);
        const char* col = barColors[(int)(rel * 4)];

        cout << "  " << Color::GRAY << setw(8) << ns[i] << Color::RESET << " | ";
        cout << col;
        for (int j = 0; j < barW; j++) cout << "\xE2\x96\x88";   // UTF-8 █
        cout << Color::RESET;
        for (int j = barW; j < 30; j++) cout << " ";
        cout << Color::GRAY
             << " med=" << setw(9) << medians[i]
             << "  avg=" << setw(9) << fixed << setprecision(0) << avgs[i]
             << Color::RESET << "\n";
    }
    cout << "  " << Color::GRAY << string(62, '-') << Color::RESET << "\n";
}

// ─── Performance Test ─────────────────────────────────────────────────────────
void performanceTest(Problem& p) {
    string title = "  PERFORMANCE ANALYSIS: " + p.getName() + "  ";
    int w = max(56, (int)title.size() + 2);
    cout << "\n" << Color::BOLD << Color::MAGENTA
         << "  " << string(w, '=') << "\n"
         << "  " << title << string(w - (int)title.size(), ' ') << "\n"
         << "  " << string(w, '=') << "\n"
         << Color::RESET;

    cout << Color::GRAY << "  Worst-case:   " << Color::RESET
         << Color::BOLD << p.getExpectedComplexity() << Color::RESET << "\n";
    cout << Color::GRAY << "  Average-case: " << Color::RESET
         << Color::BOLD << p.getAverageComplexity()  << Color::RESET << "\n\n";

    // ── Detect op-count support ──
    p.generate_input_with_size(10);
    bool hasOpCount = (p.countOps() >= 0);

    // ── Choose N range ──
    bool isLog   = (p.getExpectedComplexity().find("log") != string::npos &&
                    p.getExpectedComplexity().find("n l") == string::npos);
    bool isDijk  = (p.getName().find("Dijkstra") != string::npos ||
                    p.getName().find("Shortest") != string::npos);
    bool isGraph = (p.getName().find("Graph")    != string::npos);

    int endN = 102400;
    if (isLog)   endN = 4194304;   // go large so log growth shows in op counts
    if (isDijk)  endN = 25600;
    if (isGraph) endN = 51200;

    vector<long long> fastMedians;
    vector<double>    fastAvgs;
    vector<long long> opCounts;
    vector<int>       ns;

    // Table header
    if (hasOpCount) {
        cout << Color::GRAY << "  " << setw(10) << "n"
             << "  " << setw(10) << "ops"
             << "  " << setw(12) << "median (ns)"
             << "  " << setw(12) << "avg (ns)"
             << "\n  " << string(52, '-') << Color::RESET << "\n";
    } else {
        cout << Color::GRAY << "  " << setw(10) << "n"
             << "  " << setw(12) << "median (ns)"
             << "  " << setw(12) << "avg (ns)"
             << "  runs\n  " << string(50, '-') << Color::RESET << "\n";
    }

    for (int n = 100; n <= endN; n *= 2) {
        p.generate_input_with_size(n);

        // Timing
        vector<long long> runs;
        int batch = (n < 1000 ? 200 : n < 10000 ? 50 : 10);
        volatile int dummy = 0;
        for (int i = 0; i < 15; i++) {
            long long t = measure([&]() {
                for (int k = 0; k < batch; k++) {
                    string res = p.fast();
                    if (res.length() > 1000000) dummy++;
                }
            });
            runs.push_back(t / batch);
        }
        long long med = getMedian(runs);
        double    avg = getMean(runs);
        fastMedians.push_back(med);
        fastAvgs.push_back(avg);

        // Op count (run once — deterministic for a given input)
        long long ops = hasOpCount ? p.countOps() : -1;
        opCounts.push_back(ops);
        ns.push_back(n);

        // Row coloring: green → yellow → red
        float rel = (float)ns.size() / 10.0f;
        const char* col = rel < 0.4 ? Color::GREEN : rel < 0.7 ? Color::YELLOW : Color::RED;

        if (hasOpCount) {
            cout << "  " << col << setw(10) << n << Color::RESET
                 << "  " << Color::CYAN  << setw(10) << ops   << Color::RESET
                 << "  " << Color::BOLD  << setw(12) << med   << Color::RESET << " ns"
                 << "  " << setw(12) << fixed << setprecision(0) << avg << " ns\n";
        } else {
            cout << "  " << col << setw(10) << n << Color::RESET
                 << "  " << Color::BOLD << setw(12) << med << Color::RESET << " ns"
                 << "  " << setw(12) << fixed << setprecision(0) << avg << " ns"
                 << "  (x" << batch << ")\n";
        }
    }

    // ── Chart: use ops if available, else timing ──
    if (hasOpCount) {
        vector<double> opAvgs(opCounts.begin(), opCounts.end());
        drawChart(ns, opCounts, opAvgs, "ops");
    } else {
        drawChart(ns, fastMedians, fastAvgs, "ns");
    }

    // ── Complexity estimation: prefer op counts ──
    const vector<long long>& forEst = hasOpCount ? opCounts : fastMedians;
    EstimationResult est = estimateComplexity(ns, forEst, hasOpCount);

    // Save CSV
    string filename = "data/results_" + p.getName() + ".csv";
    replace(filename.begin(), filename.end(), ' ', '_');
    ofstream csv(filename);
    csv << "n,median_ns,avg_ns" << (hasOpCount ? ",ops" : "") << "\n";
    for (size_t i = 0; i < ns.size(); i++) {
        csv << ns[i] << "," << fastMedians[i] << ","
            << (long long)fastAvgs[i];
        if (hasOpCount) csv << "," << opCounts[i];
        csv << "\n";
    }
    csv.close();

    bool match = fuzzyMatch(est.complexity, p.getExpectedComplexity());

    // ── Final Report ──
    cout << "\n  " << Color::BOLD << Color::BLUE << "FINAL ANALYSIS REPORT\n"
         << Color::RESET << Color::GRAY << "  " << string(54, '=') << Color::RESET << "\n";

    auto row = [](const string& label, const string& val, const char* col = "") {
        cout << "  " << Color::GRAY << setw(26) << left << label
             << Color::RESET << col << val << Color::RESET << "\n";
    };

    row("Problem:",           p.getName());
    row("Worst-case (exp):",  p.getExpectedComplexity());
    row("Average-case:",      p.getAverageComplexity());
    row("Based on:",          est.usedOpCount
                                 ? "Operation counts (exact)"
                                 : "Wall-clock timing");
    row("Observed:",          est.complexity, Color::BOLD);
    row("Confidence:",        est.confidence,
        est.confidence == "High"   ? Color::GREEN  :
        est.confidence == "Medium" ? Color::YELLOW : Color::RED);

    // Doubling ratio with ideal target
    {
        auto ideal = [](const string& exp) -> string {
            if (exp == "O(1)")       return "1.00x";
            if (exp == "O(log n)")   return "~1.05-1.10x";
            if (exp == "O(n)")       return "2.00x";
            if (exp == "O(n log n)") return "~2.10-2.20x";
            if (exp == "O(E log V)") return "~2.20-2.50x";
            if (exp == "O(n^2)")     return "4.00x";
            if (exp == "O(n^3)")     return "8.00x";
            return "varies";
        };
        ostringstream oss;
        oss << fixed << setprecision(2) << est.avgRatio
            << "x  (ideal for " << p.getExpectedComplexity()
            << ": " << ideal(p.getExpectedComplexity()) << ")";
        row("Doubling ratio:", oss.str());
    }

    row("CSV saved:", filename);

    cout << "  " << Color::GRAY << setw(26) << left << "Conclusion:" << Color::RESET;
    if (match)
        cout << Color::GREEN << Color::BOLD << "  \xE2\x9C\x93 Matches expected complexity\n" << Color::RESET;
    else
        cout << Color::RED   << Color::BOLD << "  \xE2\x9C\x97 Deviates from expected\n"      << Color::RESET;

    cout << Color::GRAY << "  " << string(54, '=') << Color::RESET << "\n";
}