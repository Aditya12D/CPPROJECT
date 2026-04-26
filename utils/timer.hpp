#pragma once
#include <chrono>
#include<functional>

long long measure(function<void()> fn) {
    auto start = chrono::high_resolution_clock::now();
    fn();
    auto end = chrono::high_resolution_clock::now();

    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}