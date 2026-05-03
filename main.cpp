#include "problems.hpp"
#include "core/runner.cpp"
#include "core/performance.cpp"
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

void displayMenu() {
    cout << "\n========================================\n";
    cout << "   🚀 ALGORITHM MINI-LAB (LOCAL) 🚀   \n";
    cout << "========================================\n";
    
    auto& registry = ProblemRegistry::getInstance();
    auto problems = registry.getAllProblems();

    int i = 1;
    cout << left << setw(5) << "ID" << setw(30) << "Problem Name" << "Difficulty" << endl;
    cout << string(50, '-') << endl;

    for (auto const& pair : problems) {
        cout << left << setw(5) << i++ << setw(30) << pair.first << pair.second->getDifficulty() << endl;
    }
    cout << "0. Exit\n";
    cout << "----------------------------------------\n";
    cout << "Select a problem ID: ";
}

int main() {
    registerAllProblems();
    
    auto& registry = ProblemRegistry::getInstance();
    auto names = registry.getProblemNames();

    while (true) {
        displayMenu();
        int choice;
        if (!(cin >> choice)) break;
        if (choice == 0) break;
        if (choice < 1 || choice > names.size()) {
            cout << "Invalid choice!\n";
            continue;
        }

        string selectedName = names[choice - 1];
        auto p = registry.getProblem(selectedName);

        cout << "\n--- " << selectedName << " ---\n";
        cout << "Description: " << p->getDescription() << endl;
        cout << "Target Complexity: " << p->getExpectedComplexity() << endl;
        cout << "\nChoose Mode:\n";
        cout << "1. Stress Test (Correctness)\n";
        cout << "2. Performance Test (Analysis)\n";
        cout << "3. Replay Last Failure\n";
        cout << "0. Back\n";
        cout << "Choice: ";

        int mode;
        cin >> mode;

        if (mode == 1) {
            int tests;
            cout << "Number of tests (default 1000): ";
            cin >> tests;
            stressTest(*p, tests);
        } else if (mode == 2) {
            performanceTest(*p);
        } else if (mode == 3) {
            ifstream fin("data/fail_input.txt");
            if (!fin) {
                cout << "No failure log found.\n";
            } else {
                string input((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
                p->loadInput(input);
                cout << "Replaying with input:\n" << input << endl;
                cout << "Brute Output: " << p->brute() << endl;
                cout << "Fast Output:  " << p->fast() << endl;
            }
        }
    }

    cout << "Happy Coding! 👋\n";
    return 0;
}