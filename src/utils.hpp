#pragma once

#include <bitset>
#include <iostream>

using namespace std;

namespace Utils {
    size_t idx_nth_true_bit(const bitset<9> b, size_t n) {
        size_t i = 0;
        ++n;
        for (; i < 9; ++i) {
            n -= b.test(i);
            if (!n) {
                break;
            }
        }

        return i;
    }
}