#pragma once
#include "../utils/rng.hpp"

vector<pair<int,int>> genTree(int n) {
    vector<pair<int,int>> edges;

    for(int i=2;i<=n;i++) {
        int parent = getInt(1, i-1);
        edges.push_back({parent, i});
    }

    return edges;
}