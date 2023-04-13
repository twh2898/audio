#pragma once

#include "plugin.hpp"

namespace sound::plugins {
    class Delay : public Plugin {
        float delaySignal(const float * in, float signal, int sample) {
            if (sample >= delay) {
                return signal * (1.0 / mix) + (in[sample - delay] * feedback) * mix;
            }
            else {
                return signal;
            }
        }

    public:
        int delay;
        float feedback;
        float mix;

        Delay(Context & ctx, int delay, float feedback, float mix)
            : Plugin(ctx), delay(delay), feedback(feedback), mix(mix) {}

        void process(const float * in, float * out, size_t len) override {
            for (int i = 0; i < len; i++) {
                out[i] = delaySignal(in, in[i], i);
            }
        }
    };
}
