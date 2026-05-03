#pragma once
#include "base_generator.hpp"

namespace Generator {
    enum ArrayMode { RANDOM, SORTED, REVERSE, ALMOST_SORTED, CONSTANT };

    inline std::vector<int> genArray(int n, int minVal = -100, int maxVal = 100, ArrayMode mode = RANDOM) {
        std::vector<int> a(n);
        for(int i=0; i<n; i++) a[i] = randomInt(minVal, maxVal);

        if(mode == SORTED) {
            std::sort(a.begin(), a.end());
        }
        else if(mode == REVERSE) {
            std::sort(a.rbegin(), a.rend());
        }
        else if(mode == ALMOST_SORTED) {
            std::sort(a.begin(), a.end());
            int swaps = std::max(1, n / 10);
            for(int i=0; i<swaps; i++) {
                int u = randomInt(0, n-1);
                int v = randomInt(0, n-1);
                std::swap(a[u], a[v]);
            }
        }
        else if(mode == CONSTANT) {
            int val = randomInt(minVal, maxVal);
            std::fill(a.begin(), a.end(), val);
        }

        return a;
    }
}