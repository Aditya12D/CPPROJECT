#pragma once
#include "../utils/rng.hpp"
#include <vector>
#include <algorithm>

namespace Generator {
    // 🔹 Random integer in range [l, r]
    inline int randomInt(int l, int r) {
        return getInt(l, r);
    }

    // 🔹 Shuffle a vector
    template<typename T>
    void shuffle(std::vector<T> &v) {
        std::shuffle(v.begin(), v.end(), rng);
    }

    // 🔹 Generate a random string of length n
    inline std::string randomString(int n, char l = 'a', char r = 'z') {
        std::string s = "";
        for(int i=0; i<n; i++) s += (char)randomInt(l, r);
        return s;
    }
}
