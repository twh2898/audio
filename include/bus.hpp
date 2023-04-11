#pragma once

#include <vector>

#include "plugins/plugin.hpp"

namespace sound {
    using std::vector;

    class Bus {
        size_t channels;

        vector<Plugin *> sources;
        vector<Plugin *> effects;

    public:
        Bus(size_t channels = 1) : channels(channels) {}

        Bus(Bus && other)
            : sources(move(other.sources)), effects(move(other.effects)) {}

        Bus & operator=(Bus && other) {
            sources = move(other.sources);
            effects = move(other.effects);
            return *this;
        }

        Bus(const Bus &) = delete;
        Bus & operator=(const Bus &) = delete;

        ~Bus() {}

        void addSource(Plugin & source) {
            sources.push_back(&source);
        }

        void addEffect(Plugin & effect) {
            effects.push_back(&effect);
        }

        void process(const float ** in, float ** out, size_t len, size_t channels) {
            for (auto * src : sources) {
                src->process(in, out, len, channels);
            }
        }

        void processReplace(const float ** in,
                            float ** out,
                            size_t len,
                            size_t channels) {
            for (auto * eff : effects) {
                eff->processReplace(in, out, len, channels);
            }
        }
    };
}
