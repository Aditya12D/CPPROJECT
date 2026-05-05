#pragma once
#include "../utils/logger.hpp"
#include "../utils/timer.hpp"
#include "../utils/colors.hpp"
#include "problem.hpp"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <climits>

using namespace std;
using namespace chrono;

// ─── Progress Bar ─────────────────────────────────────────────────────────
void drawProgress(int t, int total, double elapsedMs) {
    float pct  = (float)t / total * 100.0f;
    int filled = (int)(t * 25.0f / total);

    cout << "\r  " << Color::DIM << "[" << Color::RESET;
    for (int i = 0; i < 25; i++)
        cout << (i < filled ? (string(Color::GREEN) + "=" + Color::RESET)
                            : (string(Color::GRAY)  + "-" + Color::RESET));
    cout << Color::DIM << "]" << Color::RESET;
    cout << " " << Color::BOLD << setw(3) << (int)pct << "%" << Color::RESET;
    cout << Color::GRAY << "  (" << t << "/" << total << ")" << Color::RESET;

    if (t > 0 && t < total) {
        double eta = (elapsedMs / t) * (total - t) / 1000.0;
        cout << Color::YELLOW << "  ETA " << fixed << setprecision(1) << eta << "s" << Color::RESET;
    }
    cout << flush;
}

// ─── Failure Shrinking ────────────────────────────────────────────────────
struct FailInfo { string input, brute, fast; };

FailInfo shrinkFailure(Problem& p, const FailInfo& original) {
    FailInfo best = original;
    int bestSize  = INT_MAX;

    cout << "\n  " << Color::CYAN << Color::BOLD
         << "  Shrinking to minimal failing case..." << Color::RESET << "\n";

    for (int size = 8; size >= 1; size--) {
        for (int attempt = 0; attempt < 300; attempt++) {
            p.generate_input_with_size(size);
            string b = p.brute();
            string f = p.fast();
            if (b != f && size < bestSize) {
                best    = { p.getInput(), b, f };
                bestSize = size;
                break;
            }
        }
    }

    if (bestSize < INT_MAX)
        cout << "  " << Color::CYAN << "  Shrunk to size ~" << bestSize << Color::RESET << "\n";
    else
        cout << "  " << Color::YELLOW << "  Could not shrink further." << Color::RESET << "\n";

    return best;
}

// ─── Stress Test ──────────────────────────────────────────────────────────
void stressTest(Problem& p, int tests, int timeLimitMs = 5000) {
    // ── Header ──
    string title = "  STRESS TEST: " + p.getName() + "  ";
    int w = max(48, (int)title.size() + 2);
    cout << "\n" << Color::BOLD << Color::BLUE
         << "  " << string(w, '=') << "\n"
         << "  " << title << string(w - (int)title.size(), ' ') << "\n"
         << "  " << string(w, '=') << "\n"
         << Color::RESET;

    cout << Color::GRAY
         << "  Tests: " << tests
         << "  |  Time limit: " << timeLimitMs << "ms/test\n\n"
         << Color::RESET;

    auto globalStart = high_resolution_clock::now();
    int  tleFails    = 0;

    for (int t = 1; t <= tests; t++) {
        p.generate_input();

        auto t1 = high_resolution_clock::now();
        string b = p.brute();
        auto t2 = high_resolution_clock::now();
        string f = p.fast();
        auto t3 = high_resolution_clock::now();

        double fastMs = duration_cast<microseconds>(t3 - t2).count() / 1000.0;

        // ── TLE check ──
        if (fastMs > timeLimitMs) {
            if (++tleFails == 1)
                cout << "\n";
            cout << "  " << Color::YELLOW << Color::BOLD
                 << "  TLE at test #" << t << " — fast() took "
                 << fixed << setprecision(1) << fastMs << "ms"
                 << Color::RESET << "\n";
            if (tleFails >= 3) {
                cout << "  " << Color::YELLOW << "  Too many TLEs. Aborting.\n" << Color::RESET;
                return;
            }
            continue;
        }

        // ── Correctness check ──
        if (b != f) {
            FailInfo orig = { p.getInput(), b, f };
            FailInfo best = shrinkFailure(p, orig);

            cout << "\n  " << Color::RED << Color::BOLD
                 << "  MISMATCH at test #" << t << Color::RESET << "\n"
                 << Color::GRAY << "  " << string(46, '-') << Color::RESET << "\n";

            // Show input (truncated)
            string preview = best.input.substr(0, 180);
            if (best.input.size() > 180) preview += "...";
            cout << "  " << Color::BOLD << "Input:\n" << Color::RESET
                 << Color::GRAY << "  " << preview << "\n" << Color::RESET;

            cout << "  " << Color::GREEN << "  Brute: " << Color::RESET << Color::BOLD << best.brute << "\n" << Color::RESET;
            cout << "  " << Color::RED   << "  Fast:  " << Color::RESET << Color::BOLD << best.fast  << "\n" << Color::RESET;

            logFailure(best.input, best.brute, best.fast);
            cout << "  " << Color::CYAN << "  Saved to data/fail_input.txt for replay.\n" << Color::RESET;
            return;
        }

        // ── Progress bar ──
        auto now = high_resolution_clock::now();
        double elapsedMs = duration_cast<milliseconds>(now - globalStart).count();
        if (tests >= 10 && (t % max(1, tests / 20) == 0 || t == tests))
            drawProgress(t, tests, elapsedMs);
    }

    auto end     = high_resolution_clock::now();
    double elapsed = duration_cast<milliseconds>(end - globalStart).count() / 1000.0;
    int    tps     = (int)(tests / (elapsed + 0.001));

    cout << "\n\n  " << Color::GREEN << Color::BOLD
         << "  ALL " << tests << " TESTS PASSED!\n" << Color::RESET
         << Color::GRAY
         << "  Speed: " << tps << " tests/sec  |  Total: "
         << fixed << setprecision(2) << elapsed << "s\n"
         << Color::RESET;
}