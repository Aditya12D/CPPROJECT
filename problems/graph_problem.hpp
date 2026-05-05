#pragma once
#include "../core/problem.hpp"
#include "../generators/graph_generator.hpp"
#include <vector>
#include <queue>
#include <sstream>
#include <set>

using namespace std;

struct GraphProblem : Problem {
    string getName() const override { return "Shortest Path (Unweighted)"; }
    string getDescription() const override { return "Find the distance from node 1 to node n in a graph."; }
    string getDifficulty() const override { return "Medium"; }
    string getExpectedComplexity() const override { return "O(V + E)"; }

    int n, m;
    vector<vector<int>> adj;

    void generate_input() override {
        n = Generator::randomInt(5, 20);
        m = Generator::randomInt(n-1, n*2);
        auto edges = Generator::genEdges(n, m);
        adj.assign(n + 1, {});
        for(auto& e : edges) {
            adj[e.first].push_back(e.second);
            adj[e.second].push_back(e.first);
        }
    }

    void generate_input_with_size(int size) override {
        n = size;
        m = size * 2;
        auto edges = Generator::genEdges(n, m);
        adj.assign(n + 1, {});
        for(auto& e : edges) {
            adj[e.first].push_back(e.second);
            adj[e.second].push_back(e.first);
        }
    }

    // brute BFS (inefficient)
    string brute() override {
        vector<int> dist(n+1, 1e9);
        dist[1] = 0;

        for(int i=1;i<=n;i++) {
            for(int u=1;u<=n;u++) {
                for(int v : adj[u]) {
                    dist[v] = min(dist[v], dist[u] + 1);
                }
            }
        }

        return (dist[n] >= 1e9 ? "-1" : to_string(dist[n]));
    }

    // fast BFS
    string fast() override {
        vector<int> dist(n+1, -1);
        queue<int> q;

        dist[1] = 0;
        q.push(1);

        while(!q.empty()) {
            int u = q.front(); q.pop();

            for(int v : adj[u]) {
                if(dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }

        return to_string(dist[n]);
    }

    // Count BFS ops: 1 per vertex dequeued + 1 per edge examined = O(V + E)
    long long countOps() override {
        vector<bool> visited(n + 1, false);
        queue<int> q;
        long long ops = 0;

        visited[1] = true;
        q.push(1);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            ops++;                       // vertex dequeue: O(1)
            for (int v : adj[u]) {
                ops++;                   // edge examination: O(1)
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        return ops;                      // ≈ V + E
    }

    string getInput() override {
        string s = to_string(n) + " " + to_string(m) + "\n";
        set<pair<int,int>> printed;
        for(int u = 1; u <= n; u++) {
            for(int v : adj[u]) {
                pair<int,int> e = {min(u,v), max(u,v)};
                if(printed.count(e)) continue;
                printed.insert(e);
                s += to_string(u) + " " + to_string(v) + "\n";
            }
        }
        return s;
    }

    void loadInput(const string &input) override {
        stringstream ss(input);
        ss >> n >> m;
        adj.assign(n+1, {});
        for(int i=0;i<m;i++) {
            int u,v;
            ss >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
    }
};