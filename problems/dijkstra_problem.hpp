#pragma once
#include "../core/problem.hpp"
#include "../generators/weighted_graph_generator.hpp"
#include <vector>
#include <queue>
#include <sstream>

using namespace std;

struct DijkstraProblem : Problem {
    int n, m;
    struct Edge { int to, w; };
    vector<vector<Edge>> adj;

    string getName() const override { return "Dijkstra (Shortest Path)"; }
    string getDescription() const override { return "Find the shortest path from node 1 to node n in a weighted graph."; }
    string getDifficulty() const override { return "Hard"; }
    string getExpectedComplexity() const override { return "O(E log V)"; }
    string getAverageComplexity()  const override { return "O(E log V)"; }

    void generate_input() override {
        n = Generator::randomInt(5, 20);
        m = Generator::randomInt(n - 1, n * 2);
        auto edges = Generator::genWeightedEdges(n, m);
        adj.assign(n + 1, {});
        for (auto& e : edges) {
            adj[e.u].push_back({e.v, e.w});
            adj[e.v].push_back({e.u, e.w});
        }
    }

    void generate_input_with_size(int size) override {
        n = size;
        // Use E = n * log2(n) so the "E log V" term is visible in timing
        int logN = max(1, (int)log2((double)size));
        m = min(n * logN, n * (n - 1) / 2); // cap at max edges
        auto edges = Generator::genWeightedEdges(n, m);
        adj.assign(n + 1, {});
        for (auto& e : edges) {
            adj[e.u].push_back({e.v, e.w});
            adj[e.v].push_back({e.u, e.w});
        }
    }

    // 🔹 brute: O(V^2) Dijkstra
    string brute() override {
        vector<long long> dist(n + 1, 1e18);
        vector<bool> visited(n + 1, false);
        dist[1] = 0;

        for (int i = 0; i < n; i++) {
            int u = -1;
            for (int j = 1; j <= n; j++) {
                if (!visited[j] && (u == -1 || dist[j] < dist[u])) u = j;
            }

            if (dist[u] == 1e18) break;
            visited[u] = true;

            for (auto& e : adj[u]) {
                if (dist[u] + e.w < dist[e.to]) {
                    dist[e.to] = dist[u] + e.w;
                }
            }
        }

        return (dist[n] == 1e18 ? "-1" : to_string(dist[n]));
    }

    // 🔹 fast: O(E log V) Dijkstra
    string fast() override {
        vector<long long> dist(n + 1, 1e18);
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;

        dist[1] = 0;
        pq.push({0, 1});

        while (!pq.empty()) {
            long long d = pq.top().first;
            int u = pq.top().second;
            pq.pop();

            if (d > dist[u]) continue;

            for (auto& e : adj[u]) {
                if (dist[u] + e.w < dist[e.to]) {
                    dist[e.to] = dist[u] + e.w;
                    pq.push({dist[e.to], e.to});
                }
            }
        }

        return (dist[n] == 1e18 ? "-1" : to_string(dist[n]));
    }

    // Count weighted ops: edge scans = 1, PQ push/pop = log2(V) each
    // This makes total ops reflect O(E log V) rather than just O(E)
    long long countOps() override {
        int logV = max(1, (int)ceil(log2((double)max(n, 2))));
        vector<long long> dist(n + 1, (long long)1e18);
        priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<pair<long long,int>>> pq;
        dist[1] = 0;
        pq.push({0, 1});
        long long ops = logV;  // initial push
        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            ops += logV;              // PQ pop costs O(log V)
            if (d > dist[u]) continue;
            for (auto& e : adj[u]) {
                ops++;                // edge scan: O(1)
                if (dist[u] + e.w < dist[e.to]) {
                    dist[e.to] = dist[u] + e.w;
                    pq.push({dist[e.to], e.to});
                    ops += logV;      // PQ push costs O(log V)
                }
            }
        }
        return ops;
    }

    string getInput() override {
        string s = to_string(n) + " " + to_string(m) + "\n";
        for (int u = 1; u <= n; u++) {
            for (auto& e : adj[u]) {
                if (u < e.to) s += to_string(u) + " " + to_string(e.to) + " " + to_string(e.w) + "\n";
            }
        }
        return s;
    }

    void loadInput(const string &input) override {
        stringstream ss(input);
        ss >> n >> m;
        adj.assign(n + 1, {});
        for (int i = 0; i < m; i++) {
            int u, v, w;
            ss >> u >> v >> w;
            adj[u].push_back({v, w});
            adj[v].push_back({u, w});
        }
    }
};
