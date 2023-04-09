#pragma once

#include <cmath>
#include <iostream>

namespace sound {
    const float a = std::pow(2.0, 1.0 / 12.0);

    float note(int n, int a4 = 440) {
        return a4 * std::pow(a, n);
    }

    float dcomp(float v, float a = 3.0) {
        return 2 / (1 + std::pow(a, -v)) - 1;
        // return v / (v + a);
    }

    float scomp(float v) {
        return std::tanh(v);
    }

    namespace wave {
        float sint(int n, float t) {
            // float spc = t / note(n);
            return std::sin((t * note(n)) * 2 * M_PI);
        }

        float sin(int i, int n, int samp) {
            float spc = samp / note(n);
            return std::sin((i / spc) * 2 * M_PI);
        }

        float sawt(int n, float t) {
            float ival;
            return std::modf(t * note(n), &ival);
        }

        float saw(int i, int n, int samp) {
            float spc = samp / note(n);
            float ival;
            return std::modf(i / spc, &ival);
        }

        float trit(int n, float t) {
            return std::abs(sawt(n, t) - 0.5) * 2;
        }

        float tri(int i, int n, int samp) {
            return std::abs(saw(i, n, samp) - 0.5) * 2;
        }

        float sqrt(int n, float t, float duty = 0.5) {
            return sawt(n, t) > duty;
        }

        float sqr(int i, int n, int samp, float duty = 0.5) {
            return saw(i, n, samp) > duty;
        }
    }
}
