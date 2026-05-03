#pragma once
#include "base_generator.hpp"
#include <vector>
#include <algorithm>

namespace Generator {
    // 🔹 Generate a DAG with n nodes and m edges
    inline std::vector<std::pair<int, int>> genDAG(int n, int m) {
        std::vector<std::pair<int, int>> edges;
        
        // 🔹 In a DAG, if we ensure u < v, we guarantee no cycles
        std::vector<std::pair<int, int>> all_possible;
        for (int i = 1; i <= n; i++) {
            for (int j = i + 1; j <= n; j++) {
                all_possible.push_back({i, j});
            }
        }

        std::shuffle(all_possible.begin(), all_possible.end(), rng);

        int actual_m = std::min((int)all_possible.size(), m);
        for (int i = 0; i < actual_m; i++) {
            edges.push_back(all_possible[i]);
        }

        return edges;
    }

    // 🔹 Generate DAG by density (0.0 to 1.0)
    inline std::vector<std::pair<int, int>> genDAGByDensity(int n, double density) {
        long long max_edges = (long long)n * (n - 1) / 2;
        int m = (int)(max_edges * density);
        return genDAG(n, m);
    }
}
