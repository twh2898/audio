#pragma once

namespace sound {
    class Context {
    public:
        int sampleRate;
        float tuning;

        Context(int sampleRate, float tuning = 440.0)
            : sampleRate(sampleRate), tuning(tuning) {}

        ~Context() {}

        Context(const Context & other) = default;
        Context(Context && other) = default;

        Context & operator=(const Context & other) = default;
        Context & operator=(Context && other) = default;
    };
}
