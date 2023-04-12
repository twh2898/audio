#pragma once

#include "plugin.hpp"
#include "util.hpp"

namespace sound::plugins {
    class Oscillator : public Plugin {
        inline float noteHz() {
            return util::note(note) + detune;
        }

        inline float spc() {
            return ctx.sampleRate / noteHz();
        }

        inline float sinWave(int sample) {
            return std::sin((sample / spc()) * 2 * M_PI);
        }

        inline float sawWave(int sample) {
            float ival = 0;
            return 2.0 * std::modf(sample / spc(), &ival) - 1.0;
        }

        inline float triWave(int sample) {
            return 2.0 * std::abs(sawWave(sample)) - 1.0;
        }

        inline float sqrWave(int sample) {
            return sawWave(sample) > duty ? 1.0 : -1.0;
        }

        float createWave(int sample) {
            switch (shape) {
                case Sin:
                    return sinWave(sample);
                case Saw:
                    return sawWave(sample);
                case Triangle:
                    return triWave(sample);
                case Square:
                    return sqrWave(sample);
            }

            return 0.0f;
        }

    public:
        float detune;
        float gain;
        float duty;
        int note;
        bool active;

        enum Shape {
            Sin,
            Saw,
            Triangle,
            Square,
        } shape;

        Oscillator(Context & ctx, Shape shape = Sin)
            : Plugin(ctx),
              shape(shape),
              detune(0.0),
              gain(1.0),
              duty(0.5),
              note(0),
              active(false) {}

        virtual ~Oscillator() {}

        inline void pass(const float ** in, float ** out, size_t len, size_t channels) {
            for (int i = 0; i < len; i++) {
                for (int c = 0; c < channels; c++) {
                    out[c][i] = in[c][i];
                }
            }
        }

        void process(const float ** in, float ** out, size_t len, size_t channels) override {
            if (!active) {
                pass(in, out, len, channels);
                return;
            }
            for (int i = 0; i < len; i++) {
                for (int c = 0; c < channels; c++) {
                    out[c][i] = in[c][i] + createWave(i) * gain;
                }
            }
        }

        void processReplace(const float ** in,
                            float ** out,
                            size_t len,
                            size_t channels) override {
            if (!active) {
                pass(in, out, len, channels);
                return;
            }
            for (int i = 0; i < len; i++) {
                for (int c = 0; c < channels; c++) {
                    out[c][i] = createWave(i) * gain;
                }
            }
        }
    };

}
