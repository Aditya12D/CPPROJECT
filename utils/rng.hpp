#pragma once
#include <random>
#include <chrono>
#include <functional>
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

int getInt(int l, int r) {
    return uniform_int_distribution<int>(l, r)(rng);
}
void setSeed(int seed) {
    rng.seed(seed);
}