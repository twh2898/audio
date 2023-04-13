#pragma once

#include "plugin.hpp"

namespace sound::plugins {
    class Gain : public Plugin {
    public:
        float gain;

        Gain(Context & ctx, float gain = 1.0) : Plugin(ctx), gain(gain) {}

        void process(const float * in, float * out, size_t len) override {
            for (int i = 0; i < len; i++) {
                out[i] = in[i] * gain;
            }
        }
    };
}
