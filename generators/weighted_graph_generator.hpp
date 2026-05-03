#pragma once
#include "graph_generator.hpp"

namespace Generator {
    struct WeightedEdge {
        int u, v, w;
    };

    inline std::vector<WeightedEdge> genWeightedEdges(int n, int m, int minW = 1, int maxW = 100, GraphMode mode = CONNECTED_GRAPH) {
        auto edges = genEdges(n, m, mode);
        std::vector<WeightedEdge> weighted;
        
        for (auto& e : edges) {
            weighted.push_back({e.first, e.second, getInt(minW, maxW)});
        }
        
        return weighted;
    }
}
