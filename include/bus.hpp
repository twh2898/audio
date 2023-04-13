#pragma once

#include <vector>

#include "plugins/plugin.hpp"

namespace sound {
    using std::vector;

    class Bus {
        Context & ctx;

        vector<Plugin *> sources;
        vector<Plugin *> effects;

        vector<float> buffer;
        vector<float> tmp;

        void clearBuffers() {
            for (int i = 0; i < buffer.size(); i++) {
                buffer[i] = tmp[i] = 0;
            }
        }

        void sumBuffer() {
            for (int i = 0; i < buffer.size(); i++) {
                buffer[i] += tmp[i];
                tmp[i] = 0;
            }
        }

        void copyBuffer() {
            for (int i = 0; i < buffer.size(); i++) {
                buffer[i] = tmp[i];
                tmp[i] = 0;
            }
        }

    public:
        Bus(Context & ctx)
            : ctx(ctx), buffer(ctx.sampleRate, 0), tmp(ctx.sampleRate, 0) {}

        const vector<float> & getBuffer() {
            return buffer;
        }

        void addSource(Plugin & source) {
            sources.push_back(&source);
        }

        void addEffect(Plugin & effect) {
            effects.push_back(&effect);
        }

        void process() {
            buffer.resize(ctx.sampleRate, 0.0);
            tmp.resize(ctx.sampleRate, 0.0);
            clearBuffers();

            for (auto * src : sources) {
                // I know this is going to backfire, but I'd rather deal with
                // tracing this than tracing a source that uses it's input.
                src->process(nullptr, tmp.data(), buffer.size());
                sumBuffer();
            }

            for (auto * eff : effects) {
                eff->process(buffer.data(), tmp.data(), buffer.size());
                copyBuffer();
            }
        }
    };
}
