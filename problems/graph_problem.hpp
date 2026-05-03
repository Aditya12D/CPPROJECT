#pragma once
#include "../core/problem.hpp"
#include "../generators/graph_generator.hpp"
#include <vector>
#include <queue>
#include <sstream>
using namespace std;

struct GraphProblem : Problem {
    int n, m;
    vector<vector<int>> adj;

    void generate_input() override {
        n = getInt(5, 20);
        m = getInt(n-1, n*2);
        adj = genGraph(n, m);
    }

    void generate_input_with_size(int size) override {
        n = size;
        m = size * 2;
        adj = genGraph(n, m);
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

        return to_string(dist[n]);
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