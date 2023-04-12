#pragma once

#include "plugin.hpp"

namespace sound::plugins {
    class Delay : public Plugin {
        vector<float> delayBuff;

        float delaySignal(float signal, int sample) {
            if (sample >= delay) {
                return signal * (1.0 / mix)
                       + (delayBuff[sample - delay] * feedback) * mix;
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
            : Plugin(ctx),
              delay(delay),
              feedback(feedback),
              mix(mix),
              delayBuff(delay, 0) {}

        void process(vector<float> & buff) override {
            delayBuff.resize(delay);
            for (int i = 0; i < buff.size(); i++) {
                delayBuff[i] = buff[i];
                buff[i] = delaySignal(buff[i], i);
            }
        }

        void processReplace(vector<float> & buff) override {
            delayBuff.resize(delay);
            for (int i = 0; i < buff.size(); i++) {
                delayBuff[i] = buff[i];
                buff[i] = delaySignal(buff[i], i);
            }
        }
    };
}