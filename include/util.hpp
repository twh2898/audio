#pragma once

#include <cmath>
#include <fstream>
#include <string>

namespace sound::util {
    using std::ofstream;
    using std::string;

    const float a = std::pow(2.0f, 1.0 / 12.0);

    inline float note(int n, int a4 = 440) {
        return a4 * std::pow(a, n);
    }

    constexpr float lerp(float a, float b, float t) noexcept {
        if ((a <= 0 && b >= 0) || (a >= 0 && b <= 0))
            return t * b + (1 - t) * a;

        if (t == 1)
            return b;

        const float x = a + t * (b - a);
        return (t > 1) == (b > a) ? (b < x ? x : b) : (b > x ? x : b);
    }

    void dumpWave(const string & filepath, float * wave, size_t len) {
        ofstream of(filepath);
        for (int i = 0; i < len; i++) {
            of << wave[i] << std::endl;
        }
        of.close();
    }

    void dumpWave(const string & filepath, uint8_t * wave, size_t len) {
        ofstream of(filepath);
        for (int i = 0; i < len; i++) {
            of << (int)wave[i] << std::endl;
        }
        of.close();
    }
}
