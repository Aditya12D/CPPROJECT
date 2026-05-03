#pragma once
#include "../utils/rng.hpp"
#include "base_generator.hpp"
#include <vector>
#include <set>
#include <algorithm>

namespace Generator {
    enum GraphMode { RANDOM_GRAPH, CONNECTED_GRAPH, ALMOST_TREE };

    inline std::vector<std::pair<int, int>> genEdges(int n, int m, GraphMode mode = CONNECTED_GRAPH) {
        std::set<std::pair<int, int>> edgeSet;
        std::vector<std::pair<int, int>> edges;

        auto addEdge = [&](int u, int v) {
            if (u == v) return false;
            if (u > v) std::swap(u, v);
            if (edgeSet.count({u, v})) return false;
            edgeSet.insert({u, v});
            edges.push_back({u, v});
            return true;
        };

        if (mode == CONNECTED_GRAPH || mode == ALMOST_TREE) {
            // 🔹 Step 1: Ensure connectivity with a tree
            std::vector<int> nodes(n);
            std::iota(nodes.begin(), nodes.end(), 1);
            std::shuffle(nodes.begin(), nodes.end(), rng);

            for (int i = 1; i < n; i++) {
                int parentIdx = getInt(0, i - 1);
                addEdge(nodes[i], nodes[parentIdx]);
            }
        }

        int targetM = m;
        if (mode == ALMOST_TREE) {
            targetM = n + getInt(0, std::max(0, (int)sqrt(n)));
        }

        // 🔹 Step 2: Add remaining edges
        int attempts = 0;
        while (edges.size() < targetM && attempts < targetM * 2) {
            addEdge(getInt(1, n), getInt(1, n));
            attempts++;
        }

        return edges;
    }

    // 🔹 Generate graph by density
    inline std::vector<std::pair<int, int>> genEdgesByDensity(int n, double density) {
        long long max_edges = (long long)n * (n - 1) / 2;
        int m = (int)(max_edges * density);
        return genEdges(n, m, RANDOM_GRAPH);
    }
}