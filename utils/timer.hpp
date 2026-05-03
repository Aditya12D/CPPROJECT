#pragma once
#include <chrono>
using namespace std;

template<typename Func>
long long measure(Func fn) {
    auto start = chrono::high_resolution_clock::now();
    fn();
    auto end = chrono::high_resolution_clock::now();

    return chrono::duration_cast<chrono::nanoseconds>(end - start).count();
}