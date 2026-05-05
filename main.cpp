#include "problems.hpp"
#include "core/runner.cpp"
#include "core/performance.cpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

// ─── Helpers ──────────────────────────────────────────────────────────────
static string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

static const char* diffColor(const string& d) {
    if (d == "Easy")   return Color::GREEN;
    if (d == "Medium") return Color::YELLOW;
    if (d == "Hard")   return Color::RED;
    return Color::WHITE;
}

// ─── Interactive Menu ─────────────────────────────────────────────────────
void displayMenu(const vector<string>& names, ProblemRegistry& registry) {
    cout << "\n" << Color::BOLD << Color::BLUE;
    cout << "  ╔══════════════════════════════════════════════╗\n";
    cout << "  ║          ALGORITHM MINI-LAB  (LOCAL)         ║\n";
    cout << "  ╚══════════════════════════════════════════════╝\n";
    cout << Color::RESET;

    cout << Color::GRAY
         << "  " << setw(4) << left << "ID"
         << setw(35) << "Problem"
         << setw(12) << "Difficulty"
         << "Complexity\n"
         << "  " << string(62, '-') << "\n"
         << Color::RESET;

    int i = 1;
    for (const auto& nm : names) {
        auto p   = registry.getProblem(nm);
        string diff = p->getDifficulty();
        cout << "  " << Color::BOLD << setw(4) << left << i++ << Color::RESET
             << setw(35) << nm
             << diffColor(diff) << setw(12) << diff << Color::RESET
             << Color::GRAY << p->getExpectedComplexity() << Color::RESET << "\n";
    }
    cout << "  " << Color::GRAY << "  0. Exit\n" << Color::RESET;
    cout << "  " << string(62, '-') << "\n";
    cout << "  " << Color::BOLD << "Select a problem ID: " << Color::RESET;
}

// ─── Run a problem (shared by interactive & CLI) ──────────────────────────
void runProblem(Problem& p, int mode, int tests, int timeLimitMs) {
    if (mode == 1) {
        stressTest(p, tests, timeLimitMs);
    } else if (mode == 2) {
        performanceTest(p);
    } else if (mode == 3) {
        ifstream fin("data/fail_input.txt");
        if (!fin) {
            cout << Color::YELLOW << "  No failure log found.\n" << Color::RESET;
        } else {
            string input((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
            p.loadInput(input);
            cout << Color::BOLD << "\n  Replaying failing input:\n" << Color::RESET
                 << Color::GRAY << input << Color::RESET << "\n";
            cout << Color::GREEN << "  Brute: " << Color::RESET << p.brute() << "\n";
            cout << Color::RED   << "  Fast:  " << Color::RESET << p.fast()  << "\n";
        }
    }
}

// ─── CLI Argument Parser ──────────────────────────────────────────────────
struct CLIArgs {
    string problem = "";
    int    mode    = 0;   // 0 = not set
    int    tests   = 1000;
    int    timelimit = 5000;
    bool   valid   = false;
};

CLIArgs parseCLI(int argc, char* argv[]) {
    CLIArgs a;
    for (int i = 1; i < argc; i++) {
        string flag = argv[i];
        if ((flag == "--problem" || flag == "-p") && i + 1 < argc)
            a.problem = argv[++i];
        else if ((flag == "--mode" || flag == "-m") && i + 1 < argc) {
            string m = argv[++i];
            if      (m == "stress" || m == "1") a.mode = 1;
            else if (m == "perf"   || m == "2") a.mode = 2;
            else if (m == "replay" || m == "3") a.mode = 3;
        }
        else if ((flag == "--tests" || flag == "-t") && i + 1 < argc)
            a.tests = stoi(argv[++i]);
        else if ((flag == "--timelimit" || flag == "-l") && i + 1 < argc)
            a.timelimit = stoi(argv[++i]);
    }
    a.valid = !a.problem.empty() && a.mode != 0;
    return a;
}

// ─── Main ─────────────────────────────────────────────────────────────────
int main(int argc, char* argv[]) {
    registerAllProblems();
    auto& registry = ProblemRegistry::getInstance();
    auto  names    = registry.getProblemNames();

    // ── CLI mode ──
    CLIArgs cli = parseCLI(argc, argv);
    if (cli.valid) {
        // Find problem by partial case-insensitive name match
        string query = toLower(cli.problem);
        string matched = "";
        for (const auto& nm : names) {
            if (toLower(nm).find(query) != string::npos) { matched = nm; break; }
        }
        if (matched.empty()) {
            cout << Color::RED << "  No problem matching \"" << cli.problem << "\"\n" << Color::RESET;
            cout << Color::GRAY << "  Available:\n" << Color::RESET;
            for (const auto& nm : names) cout << "    - " << nm << "\n";
            return 1;
        }
        auto p = registry.getProblem(matched);
        runProblem(*p, cli.mode, cli.tests, cli.timelimit);
        return 0;
    }

    // ── Interactive mode ──
    while (true) {
        displayMenu(names, registry);
        int choice;
        if (!(cin >> choice)) break;
        if (choice == 0) break;
        if (choice < 1 || choice > (int)names.size()) {
            cout << Color::RED << "  Invalid choice!\n" << Color::RESET;
            continue;
        }

        auto p = registry.getProblem(names[choice - 1]);
        cout << "\n  " << Color::BOLD << p->getName() << Color::RESET << "\n"
             << Color::GRAY << "  " << p->getDescription() << "\n"
             << "  Complexity: " << p->getExpectedComplexity() << "\n\n" << Color::RESET;

        cout << "  " << Color::BOLD << "Mode:\n" << Color::RESET
             << "  " << Color::GREEN  << "1." << Color::RESET << " Stress Test (Correctness)\n"
             << "  " << Color::YELLOW << "2." << Color::RESET << " Performance Test (Analysis)\n"
             << "  " << Color::CYAN   << "3." << Color::RESET << " Replay Last Failure\n"
             << "  " << Color::GRAY   << "0." << Color::RESET << " Back\n"
             << "  Choice: ";

        int mode; cin >> mode;
        if (mode == 0) continue;

        int tests = 1000, timelimit = 5000;
        if (mode == 1) {
            cout << "  Number of tests [default 1000]: ";
            if (!(cin >> tests) || tests <= 0) tests = 1000;
            cout << "  Time limit per test in ms [default 5000]: ";
            if (!(cin >> timelimit) || timelimit <= 0) timelimit = 5000;
        }

        runProblem(*p, mode, tests, timelimit);
    }

    cout << "\n  " << Color::BOLD << Color::BLUE << "Happy Coding!\n" << Color::RESET;
    return 0;
}