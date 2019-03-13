#pragma once

namespace util {
    template <class T>
    T clamp(T value, T min, T max) {
        if (min > value) {
            return min;
        } else if (max < value) {
            return max;
        } else {
            return value;
        }
    }
}
