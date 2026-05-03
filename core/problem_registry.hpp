#pragma once
#include "problem.hpp"
#include <map>
#include <memory>
#include <vector>
#include <string>

class ProblemRegistry {
    map<string, shared_ptr<Problem>> registry;

public:
    static ProblemRegistry& getInstance() {
        static ProblemRegistry instance;
        return instance;
    }

    void registerProblem(shared_ptr<Problem> p) {
        registry[p->getName()] = p;
    }

    shared_ptr<Problem> getProblem(const string& name) {
        if (registry.count(name)) return registry[name];
        return nullptr;
    }

    vector<string> getProblemNames() const {
        vector<string> names;
        for (auto const& pair : registry) {
            names.push_back(pair.first);
        }
        return names;
    }

    const map<string, shared_ptr<Problem>>& getAllProblems() const {
        return registry;
    }
};

// 🔹 Macro to register problems (helper)
#define REGISTER_PROBLEM(ProblemType) \
    ProblemRegistry::getInstance().registerProblem(make_shared<ProblemType>());
