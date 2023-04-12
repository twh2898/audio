#pragma once

#include "plugin.hpp"

namespace sound::plugins {
    class Gain : public Plugin {
    public:
        float gain;

        Gain(Context & ctx, float gain = 1.0) : Plugin(ctx), gain(gain) {}

        void process(vector<float> & buff) override {
            for (int i = 0; i < buff.size(); i++) {
                buff[i] = buff[i] * gain;
            }
        }

        void processReplace(vector<float> & buff) override {
            for (int i = 0; i < buff.size(); i++) {
                buff[i] = buff[i] * gain;
            }
        }
    };
}