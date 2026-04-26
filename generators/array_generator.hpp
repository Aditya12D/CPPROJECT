#pragma once
#include "../utils/rng.hpp"

vector<int> genArray(int n) {
    vector<int> a(n);
    for(int i=0;i<n;i++) a[i] = getInt(-100, 100);
    return a;
}