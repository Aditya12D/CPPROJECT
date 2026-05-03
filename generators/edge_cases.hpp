#pragma once
#include "base_generator.hpp"

namespace Generator {
    inline std::vector<int> genEmpty() { return {}; }
    inline std::vector<int> genSingle(int val = 0) { return {val}; }
    inline std::vector<int> genAllSame(int n, int val) { return std::vector<int>(n, val); }
    inline std::vector<int> genAlternating(int n, int val1, int val2) {
        std::vector<int> a(n);
        for(int i=0; i<n; i++) a[i] = (i % 2 == 0 ? val1 : val2);
        return a;
    }
    inline std::vector<int> genMaxMin(int n, int minVal, int maxVal) {
        std::vector<int> a(n);
        for(int i=0; i<n; i++) a[i] = (randomInt(0, 1) ? minVal : maxVal);
        return a;
    }
}
