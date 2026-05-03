#pragma once
#include "../utils/rng.hpp"
#include <vector>
#include <set>
using namespace std;

vector<vector<int>> genGraph(int n, int m) {
    vector<vector<int>> adj(n + 1);
    set<pair<int,int>> edges;

    // 🔹 Step 1: Ensure connectivity (tree)
    for(int i = 2; i <= n; i++) {
        int parent = getInt(1, i - 1);

        adj[parent].push_back(i);
        adj[i].push_back(parent);

        edges.insert({min(parent,i), max(parent,i)});
    }

    // 🔹 Step 2: Add extra edges (without duplicates)
    int extra = m - (n - 1);

    while(extra > 0) {
        int u = getInt(1, n);
        int v = getInt(1, n);

        if(u == v) continue;  // ❌ no self-loop

        pair<int,int> e = {min(u,v), max(u,v)};

        if(edges.count(e)) continue;  // ❌ no duplicate

        edges.insert(e);

        adj[u].push_back(v);
        adj[v].push_back(u);

        extra--;
    }

    return adj;
}